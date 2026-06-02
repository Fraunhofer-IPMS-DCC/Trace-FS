echo "Current status of Xilinx AXI UART Lite module"

#echo -n "Rx FIFO:  "
#busybox devmem 0xa1050000 32
#echo -n "Tx FIFO:  "
#busybox devmem 0xa1050004 32
echo -n "STAT_REG: "
busybox devmem 0xa1050008 32
echo -n "CTRL_REG: "
busybox devmem 0xa105000c 32
