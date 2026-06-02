#!/bin/bash
# Configure and start the C-Trace unit.

# C-Trace encoder register base (MMIO).
CT_BASE=0xa0900000

# Write a 32-bit value to an MMIO register and verify it read back correctly.
# Retries up to MAX_RETRIES (default 8) to work around the observed
# first-write-after-reset / partial-commit bug.
#
# Usage: verified_write ADDR VALUE [FRIENDLY_NAME [MASK [MAX_RETRIES]]]
# MASK (default 0xffffffff) selects which bits to compare — pass e.g.
# 0xfffffff7 to ignore bit 3 (Empty in trTeControl is HW-driven).
verified_write() {
    local addr="$1" val="$2" name="${3:-$addr}" mask="${4:-0xffffffff}" max_retries="${5:-8}"
    local wr_masked=$(( val & mask ))
    local readback rb_masked
    local i=1
    while [ "$i" -le "$max_retries" ]; do
        busybox devmem "$addr" 32 "$val"
        readback=$(busybox devmem "$addr")
        rb_masked=$(( readback & mask ))
        if [ "$rb_masked" -eq "$wr_masked" ]; then
            if [ "$i" -eq 1 ]; then
                printf '  OK    %s: wrote %s, readback %s\n' "$name" "$val" "$readback"
            else
                printf '  OK@%d  %s: wrote %s, readback %s\n' "$i" "$name" "$val" "$readback"
            fi
            return 0
        fi
        i=$(( i + 1 ))
    done
    printf '  FAIL  %s: wrote %s, expected %s (mask %s), readback %s (after %d retries)\n' \
        "$name" "$val" "$(printf '0x%08x' "$wr_masked")" "$mask" "$readback" "$max_retries" >&2
    return 1
}

main() {
    echo "Enable TraceUnit"

    # Step 1: Activate the trace unit (Active=1 only, other bits at reset).
    # Per the updated RDL, trTeControl now has a separate Enable bit [1:1];
    # the unit must be Active before configuration writes take effect, and
    # Enable is set last to actually start emitting trace.
    #
    # trTeControl.Empty (bit 3) is HW-driven (reflects FIFO empty state),
    # so we mask it out of every readback comparison (mask = 0xfffffff7).
    #
    # Also absorb the first-write-after-reset bus-adapter stall by priming
    # with the same value before the verified write. Earlier versions used
    # 0x00000000 as the prime, but that would put InstMode (enum: only 3
    # and 6 defined) and Format (enum: only 1 defined) into undefined
    # states. Writing the real Step 1 value twice keeps every enum legal.
    echo "> Step 1: Activate trace unit (Active=1, reset defaults)"
    busybox devmem 0xa0900000 32 0x01008069   # prime write (absorb stall)
    verified_write 0xa0900000 0x01008069 "trTeControl (Activate)" 0xfffffff7

    # Step 2: Configure trTeControl while Active=1, Enable=0.
    #   InstTracing=1, InstMode=6 (ITR_BRANCH_HIST), InhibitSrc=1,
    #   InstSyncMode=0 (ITR_SYNC_OFF),
    #   InstSyncMax=8 → sync every 2^(8+4) = 4096 hart cycles (~41 us @ 100 MHz),
    #   Format=1 (TR_FORMAT_N), Empty=1 (HW-driven, ignored on write).
    echo "> Step 2: Configure trTeControl (InstTracing=1, sync every 4096 cycles)"
    verified_write 0xa0900000 0x0180806d "trTeControl (Configure)" 0xfffffff7

    # trTeDataControl: DataImplemented=1 (RO reset), DataTracing=0 (bit [1]).
    # Data tracing on → load/store data-transfer messages are emitted in
    # addition to instruction trace. Raises TIP-FIFO pressure (more
    # ResourceFull churn) but is needed to capture data movement.
    echo "> Configure Data Tracing (DataTracing=0, disabled)"
    verified_write 0xa0900010 0x00000001 "trTeDataControl"

    # trTsControl: Active=1, Count=1, Type=2 (TR_TS_SYSTEM — from internal
    # system/bus clock), Prescale=0, Enable=1, Width=0x3f.
    echo "> Enable timestamps (TR_TS_SYSTEM, 63-bit width)"
    verified_write 0xa0900040 0x3f008023 "trTsControl"

    # Watchpoints — see "Watchpoints Memory (ACT-ST)" in the C-Trace RDL.
    echo "> Configure watchpoints (ACT-ST)"
    local daq
    daq=$(ctrace_cmd DAQ_PC_CURR NEXUS 0x42)
    ctrace_set_watchpoints \
        0xa1040000:$daq

    echo "> Readback full watchpoints table"
    ctrace_dump_watchpoints

    # Step 3: Start tracing (set Enable=1, keep Active=1 and full config).
    echo "> Step 3: Start tracing (Enable=1)"
    verified_write 0xa0900000 0x0180806f "trTeControl (Enable)" 0xfffffff7

    echo "Next: release CPU (RISC-V) from reset"
}

# =============================================================
#  C-Trace ACT-ST watchpoint helpers.
#
#  See "Watchpoints Memory (ACT-ST)" in the ct_cs_cpuif RDL
#  (modules/ctrace/rdl/ct_cs_cpuif.rdl in the fpga repo) for the
#  full SW contract: 15 entries at 0xa0904000, strictly ascending
#  + unique Addrs, program with trTeControl.Active=0.
# =============================================================

# Watchpoints memory base and depth. Base moved from +0x4000 to +0x4100
# with the 2026-04-16 bitfile/RDL update.
CT_WP_BASE=0xa0904100
CT_WP_NUM=15

# ACT-CAP Cmd enum (RDL trActCapStCmd_e).
__CTRACE_CMD_NONE=0
__CTRACE_CMD_DAQ_PC_CURR=1
__CTRACE_CMD_DAQ_PC_CURR_LAST=2
__CTRACE_CMD_DAQ_DIRECT_DATA=3
__CTRACE_CMD_DAQ_DATA=4
__CTRACE_CMD_DAQ_DADDR=5
__CTRACE_CMD_DAQ_DATA_DADDR=6
__CTRACE_CMD_DAQ_IFETCH_TH=8
__CTRACE_CMD_DAQ_DATA_RD_TH=9
__CTRACE_CMD_DAQ_DATA_WR=10
__CTRACE_CMD_DAQ_DATA_RD=11
__CTRACE_CMD_CF_SYNC=12
__CTRACE_CMD_TE=13

# ACT-CAP Sink enum (RDL trActCapStSink_e).
__CTRACE_SINK_NEXUS=0
__CTRACE_SINK_AXIS=1
__CTRACE_SINK_AXIS_NEXUS=2
__CTRACE_SINK_TE=3

__ctrace_resolve() {
    # __ctrace_resolve PREFIX value-or-name  ->  prints numeric value.
    local var="__CTRACE_${1}_${2}"
    if [[ -n "${!var:-}" ]]; then printf '%u' "${!var}"
    else                          printf '%u' "$2"
    fi
}

# Encode a 32-bit ACT-CAP Cmd word.
# Usage: CMD=$(ctrace_cmd CMD [SINK [DIRECTDATA]])
#   CMD        : enum name (e.g. DAQ_PC_CURR) or number 0..63
#   SINK       : enum name (default NEXUS)     or number 0..3
#   DIRECTDATA : hex/decimal 24-bit (default 0)
ctrace_cmd() {
    local cmd sink direct
    cmd=$(__ctrace_resolve CMD  "${1:-NONE}")
    sink=$(__ctrace_resolve SINK "${2:-NEXUS}")
    direct=$(( ${3:-0} ))
    printf '0x%08x\n' $(( ((direct & 0xffffff) << 8) | ((sink & 3) << 6) | (cmd & 63) ))
}

# Configure the full 15-slot watchpoints memory.
# Usage: ctrace_set_watchpoints PC1:CMD1 [PC2:CMD2 ...]
#   Up to 15 pairs in any order. The helper sorts them, verifies
#   uniqueness, and fills the remaining slots with ascending odd
#   sentinel PCs + Cmd=NONE (EMSA5 forces PC[0]=0, so odd Addrs
#   never match) so the final table is strictly ascending + unique.
ctrace_set_watchpoints() {
    local base=$CT_WP_BASE num=$CT_WP_NUM none_cmd=0x00000000

    (( $# <= num )) || { echo "ctrace_set_watchpoints: max $num entries, got $#" >&2; return 1; }

    local rows=() arg pc cmd
    for arg in "$@"; do
        pc=${arg%%:*}; cmd=${arg#*:}
        rows+=("$(printf '%010u %s %s' $(( pc )) "$pc" "$cmd")")
    done
    local IFS=$'\n'
    rows=( $(printf '%s\n' "${rows[@]}" | sort) )
    unset IFS

    if (( $(printf '%s\n' "${rows[@]}" | awk '{print $1}' | sort -u | wc -l) != ${#rows[@]} )); then
        echo "ctrace_set_watchpoints: duplicate PC in input" >&2
        return 1
    fi

    local nsent=$(( num - ${#rows[@]} ))
    if (( nsent > 0 && ${#rows[@]} > 0 )); then
        local smallest=${rows[0]%% *}
        local max_sent=$(( nsent * 2 - 1 ))
        if (( max_sent >= smallest )); then
            printf 'ctrace_set_watchpoints: smallest real PC 0x%x too low for %d odd sentinels\n' \
                "$smallest" "$nsent" >&2
            return 1
        fi
    fi

    local slot=0 v=1
    while (( slot < nsent )); do
        verified_write "$(printf '0x%08x' $(( base + slot*8 )))"     "$(printf '0x%08x' "$v")" "wp${slot}.Addr"
        verified_write "$(printf '0x%08x' $(( base + slot*8 + 4 )))" "$none_cmd"                "wp${slot}.Cmd"
        slot=$(( slot + 1 )); v=$(( v + 2 ))
    done
    local row rest
    for row in "${rows[@]}"; do
        rest=${row#* }; pc=${rest%% *}; cmd=${rest#* }
        verified_write "$(printf '0x%08x' $(( base + slot*8 )))"     "$pc"  "wp${slot}.Addr"
        verified_write "$(printf '0x%08x' $(( base + slot*8 + 4 )))" "$cmd" "wp${slot}.Cmd"
        slot=$(( slot + 1 ))
    done
}

# Dump all 15 slots as a readback sanity check.
ctrace_dump_watchpoints() {
    local base=$CT_WP_BASE num=$CT_WP_NUM
    local i addr cmd pc_val cmd_val
    for (( i = 0; i < num; i++ )); do
        addr=$(printf '0x%08x' $(( base + i*8 )))
        cmd=$(printf  '0x%08x' $(( base + i*8 + 4 )))
        pc_val=$(busybox devmem "$addr")
        cmd_val=$(busybox devmem "$cmd")
        printf '  slot %2d: Addr=%s  Cmd=%s\n' "$i" "$pc_val" "$cmd_val"
    done
}

main "$@"
