echo "Last saved reason for trap"

echo -n "mcause: "
busybox devmem 0xa1044000 32
echo -n "mepc:   "
busybox devmem 0xa1044004 32
