#!/usr/bin/env bash
# Decode a C-Trace using NexRv and the emsa5 disassembly files.

set -euo pipefail

BOOT_NAME="boot-emsa5"
FW_NAME="fw-emsa5"

usage() {
    echo "Usage: $(basename "$0") <bindir> <tracefile>"
    echo ""
    echo "  bindir      Directory containing ${BOOT_NAME}.dis (required) and"
    echo "              optionally ${FW_NAME}.dis for apps that run firmware"
    echo "              from BRAM in addition to the ITCM boot code."
    echo "  tracefile   C-Trace file to decode"
    echo ""
    echo "NexRv must be available in PATH."
    exit 1
}

if [ $# -ne 2 ]; then
    usage
fi

bindir="$1"
trace="$2"

# Validate inputs
if [ ! -d "$bindir" ]; then
    echo "Error: directory '$bindir' does not exist." >&2
    exit 1
fi

if [ ! -f "${bindir}/${BOOT_NAME}.dis" ]; then
    echo "Error: disassembly file '${bindir}/${BOOT_NAME}.dis' not found." >&2
    exit 1
fi

# Firmware disassembly is optional; apps that run entirely from the ITCM
# (e.g. scenarios_a3) only produce a boot image.
has_fw=0
if [ -f "${bindir}/${FW_NAME}.dis" ]; then
    has_fw=1
fi

# Name the decoded trace outputs after the firmware when present, otherwise
# after the boot binary.
if [ "$has_fw" -eq 1 ]; then
    trc_name="$FW_NAME"
else
    trc_name="$BOOT_NAME"
fi

if [ ! -f "$trace" ]; then
    echo "Error: trace file '$trace' not found." >&2
    exit 1
fi

if ! command -v NexRv >/dev/null 2>&1; then
    echo "Error: NexRv not found in PATH." >&2
    exit 1
fi

if ! command -v zstd >/dev/null 2>&1; then
    echo "Error: zstd not found in PATH." >&2
    exit 1
fi

tmpdir=$(mktemp -d)
preserve_tmpdir=0

cleanup() {
    if [ "$preserve_tmpdir" -eq 0 ]; then
        rm -rf "$tmpdir"
    else
        echo "Preserving '$tmpdir' for inspection." >&2
    fi
}
trap cleanup EXIT

# Run a command, tee its output to a log file, and on failure print the
# log tail plus an exact shell reproducer so the user can rerun it.
# Env assignments for the command are passed as VAR=value positional
# args before a '--' separator; everything after '--' is the argv.
run_logged() {
    local log_path="$1"
    local label="$2"
    shift 2

    local -a env_args=()
    while [ $# -gt 0 ] && [ "$1" != "--" ]; do
        env_args+=("$1")
        shift
    done
    if [ $# -eq 0 ]; then
        echo "run_logged: missing '--' separator" >&2
        return 2
    fi
    shift  # drop '--'
    local -a cmd_args=("$@")

    local rc=0
    if [ ${#env_args[@]} -gt 0 ]; then
        env "${env_args[@]}" "${cmd_args[@]}" >"$log_path" 2>&1 || rc=$?
    else
        "${cmd_args[@]}" >"$log_path" 2>&1 || rc=$?
    fi

    if [ "$rc" -ne 0 ]; then
        preserve_tmpdir=1
        {
            echo ""
            echo "ERROR: ${label} failed (exit ${rc})."
            echo ""
            echo "Reproduce with:"
            printf '  '
            for e in "${env_args[@]}"; do
                printf '%q ' "$e"
            done
            for a in "${cmd_args[@]}"; do
                printf '%q ' "$a"
            done
            echo ""
            echo ""
            echo "Full log: ${log_path}"
            echo "Last 40 lines of output:"
            tail -n 40 "$log_path" | sed 's/^/  /'
        } >&2
        return "$rc"
    fi
}

# Convert disassembly to pcinfo.
run_logged "${tmpdir}/conv-boot.log" "NexRv -conv (${BOOT_NAME})" -- \
    NexRv -conv -objd "${bindir}/${BOOT_NAME}.dis" \
                -pcinfo "${bindir}/${BOOT_NAME}.pcinfo"

if [ "$has_fw" -eq 1 ]; then
    run_logged "${tmpdir}/conv-fw.log" "NexRv -conv (${FW_NAME})" -- \
        NexRv -conv -objd "${bindir}/${FW_NAME}.dis" \
                    -pcinfo "${bindir}/${FW_NAME}.pcinfo"
fi

# Build combined pcinfo (boot alone when there is no firmware image).
if [ "$has_fw" -eq 1 ]; then
    cat "${bindir}/${BOOT_NAME}.pcinfo" "${bindir}/${FW_NAME}.pcinfo" \
        > "${bindir}/combined.pcinfo"
else
    cp "${bindir}/${BOOT_NAME}.pcinfo" "${bindir}/combined.pcinfo"
fi

# Decode trace.
run_logged "${tmpdir}/decode.log" "NexRv -deco" \
    "CTXP_TEXT_TRACEFILE=${tmpdir}/${trc_name}.ctxp.txt" \
    "CTXP_TRACEFILE=${tmpdir}/${trc_name}.ctxp" \
    -- \
    NexRv -deco "$trace" -pcinfo "${bindir}/combined.pcinfo" \
                -pcout "${tmpdir}/decode.pco" -all

# Move results. The binary ctxp is compressed; the human-readable text
# version is left uncompressed for manual inspection.
zstd -19 "${tmpdir}/${trc_name}.ctxp" -o "${bindir}/${trc_name}.ctxp.zst"
cp "${tmpdir}/${trc_name}.ctxp.txt" "${bindir}/${trc_name}.ctxp.txt"

echo "Done. Output in ${bindir}:"
echo "  ${trc_name}.ctxp.zst"
echo "  ${trc_name}.ctxp.txt"
echo "  ${BOOT_NAME}.pcinfo"
if [ "$has_fw" -eq 1 ]; then
    echo "  ${FW_NAME}.pcinfo"
fi
echo "  combined.pcinfo"
