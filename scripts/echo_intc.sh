echo "Current status of Xilinx AXI Interrupt Controller"

echo -n "ISR: "
busybox devmem 0xa1000000 32
echo -n "IPR: "
busybox devmem 0xa1000004 32
echo -n "IER: "
busybox devmem 0xa1000008 32
echo -n "IAR: "
busybox devmem 0xa100000c 32
echo -n "SIE: "
busybox devmem 0xa1000010 32
echo -n "CIE: "
busybox devmem 0xa1000014 32
echo -n "IVR: "
busybox devmem 0xa1000018 32
echo -n "MER: "
busybox devmem 0xa100001c 32
echo -n "IMR: "
busybox devmem 0xa1000020 32
echo -n "ILR: "
busybox devmem 0xa1000024 32
