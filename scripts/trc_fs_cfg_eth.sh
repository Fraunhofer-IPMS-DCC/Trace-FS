#!/bin/sh
# Configure TSN endpoint and TraceBus Ethernet destination.

usage() {
    echo "Usage: $0 <mac-address> <ip-address>" >&2
    echo "Example: $0 00:0a:35:15:23:56 192.168.0.111" >&2
    exit 1
}

if [ "$#" -ne 2 ]; then
    usage
fi

mac_addr=$(printf '%s' "$1" | tr '[:upper:]' '[:lower:]')
ip_addr=$2

old_ifs=$IFS
IFS=:
set -- $mac_addr
IFS=$old_ifs

if [ "$#" -ne 6 ]; then
    echo "Invalid MAC address: $mac_addr" >&2
    usage
fi

for octet in "$@"; do
    if [ "${#octet}" -ne 2 ]; then
        echo "Invalid MAC address: $mac_addr" >&2
        usage
    fi

    case "$octet" in
        *[!0-9a-f]*)
            echo "Invalid MAC address: $mac_addr" >&2
            usage
            ;;
    esac
done

mac_reg0="0x$4$3$2$1"
mac_reg1="0x$6$5"

old_ifs=$IFS
IFS=.
set -- $ip_addr
IFS=$old_ifs

if [ "$#" -ne 4 ]; then
    echo "Invalid IP address: $ip_addr" >&2
    usage
fi

ip_hex=""
for octet in "$@"; do
    case "$octet" in
        ''|*[!0-9]*)
            echo "Invalid IP address: $ip_addr" >&2
            usage
            ;;
    esac

    if [ "$octet" -lt 0 ] || [ "$octet" -gt 255 ]; then
        echo "Invalid IP address: $ip_addr" >&2
        usage
    fi

    ip_hex="${ip_hex}$(printf '%02x' "$octet")"
done

ip_reg="0x$ip_hex"

# --- TSN endpoint (0xA0800000) ---
# Control register 0x0C: read-modify-write to enable MAC TX and RX paths.
# Default after FPGA config is 0x1930. We set bits [1] (TX enable) and
# [2] (RX enable), giving 0x1936. Without these the MAC stays idle and
# nothing reaches the wire even if the TraceBus pushes frames.
echo "Cfg TSN EP"
busybox devmem 0xa080000c
busybox devmem 0xa080000c 32 0x1936

# --- TSN TraceBus (0xA0810000) ---
# The TraceBus is the IPMS-supplied IP that builds Ethernet+IPv4+UDP
# frames around the trace payload and hands them to the TSN endpoint.
echo "Cfg TraceBus"

busybox devmem 0xa081001c 32 0x5a0

busybox devmem 0xa0810030 32 0x20

# 0x00 (low 4 bytes of destination MAC, little-endian) and 0x04 (top 2
# bytes of destination MAC, padded in the low 16 bits). These two
# registers together hold the 6-byte MAC the TraceBus will use as the
# Ethernet destination on every emitted frame.
echo "> destination MAC is $mac_addr"
busybox devmem 0xa0810000 32 "$mac_reg0"
busybox devmem 0xa0810004 32 "$mac_reg1"

# 0x14: 4-byte destination IPv4 address in network byte order. Goes
# into the IP header of every emitted UDP frame.
echo "> destination IP is $ip_addr"
busybox devmem 0xa0810014 32 "$ip_reg"
