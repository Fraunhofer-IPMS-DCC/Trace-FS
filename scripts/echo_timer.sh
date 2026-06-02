echo "Current status of Xilinx AXI Timer"

echo -n "TCSR0: "
busybox devmem 0xa1010000 32
echo -n "TLR0:  "
busybox devmem 0xa1010004 32
echo -n "TCR0:  "
busybox devmem 0xa1010008 32
echo -n "TCSR1: "
busybox devmem 0xa1010010 32
echo -n "TLR1:  "
busybox devmem 0xa1010014 32
echo -n "TCR1:  "
busybox devmem 0xa1010018 32
