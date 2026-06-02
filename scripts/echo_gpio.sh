echo "Current status of Xilinx AXI GPIO module"

echo -n "GPIO_DATA: "
busybox devmem 0xa1030000 32
echo -n "GPIO_TRI:  "
busybox devmem 0xa1030004 32
echo -n "GIER:      "
busybox devmem 0xa103011c 32
echo -n "IP IER:    "
busybox devmem 0xa1030128 32
echo -n "IP ISR:    "
busybox devmem 0xa1030120 32
