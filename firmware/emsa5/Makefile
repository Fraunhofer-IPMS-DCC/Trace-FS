TARGET  = fw-emsa5
C_SRCS  = $(wildcard *.c)
HEADERS = $(wildcard *.h)
CFLAGS  = -target rv32-linux -g -Wall -Werror -Wno-error=unknown-pragmas -std=c11 -O -finline-asm -Wa,-march=rv32imd_zicsr -I./hal/
LFLAGS  = -target rv32-linux -g -Wall -Werror -std=c11 -nostdlib -static -Wa,-march=rv32imd_zicsr -Wl,-Temsa5-gcc.ld

LINK_TARGET=link
RISCV_ARCH:=rv32im
RISCV_ABI:=ilp32

host_compiler := /usr/common/compiler/xpack-riscv-none-elf-gcc-13.2.0-2/bin/riscv-none-elf-gcc
compiler := ccomp
TOOL_DIR=$(dir $(host_compiler))

TRIPLET=riscv-none-elf
BSP_BASE = ../bsp

.PHONY: all
all: asm_test.o ctrace_test.o scenarios_a3.a boot-emsa5.elf $(C_SRCS) $(HEADERS) trap.s start.s emsa5-gcc.ld
	$(compiler) $(CFLAGS) trap.s -c
	$(compiler) $(CFLAGS) start.s -c
	$(compiler) $(CFLAGS) -c fw.c
	$(compiler) $(CFLAGS) -c hal/core.c
	$(compiler) $(CFLAGS) -c hal/gpio.c
	$(compiler) $(CFLAGS) -c handler.c
	$(compiler) $(CFLAGS) -c hal/heartbeat.c
	$(compiler) $(CFLAGS) -c hal/irq.c
	$(compiler) $(CFLAGS) -c hal/led.c
	$(compiler) $(CFLAGS) -c scheduler.c
	$(compiler) $(CFLAGS) -c task_5ms.c
	$(compiler) $(CFLAGS) -c task_10ms.c
	$(compiler) $(CFLAGS) -c task_100ms.c
	$(compiler) $(CFLAGS) -c task_1000ms.c
	$(compiler) $(CFLAGS) -c hal/timer.c
	$(compiler) $(CFLAGS) -c hal/uart.c
	$(compiler) $(LFLAGS) asm_test.o ctrace_test.o start.o trap.s handler.o core.o gpio.o heartbeat.o irq.o led.o scheduler.o task_5ms.o task_10ms.o task_100ms.o task_1000ms.o scenarios_a3.a timer.o uart.o fw.o -o $(TARGET).elf

boot-emsa5.elf: boot.s emsa5-gcc.ld
	$(compiler) $(LFLAGS) boot.s -o boot-emsa5.elf

asm_test.o: asw/asm_test/full.S asw/asm_test/asm_test.h
	$(compiler) $(CFLAGS) -c $< -o asm_test.o

ctrace_test.o: asw/ctrace_test/ctrace_test.c asw/ctrace_test/ctrace_test.h
	$(compiler) $(CFLAGS) -c $< -o ctrace_test.o

scenarios_a3.a: $(wildcard asw/scenarios_a3/*.c)
	$(compiler) $(CFLAGS) -c $?
	$(TOOL_DIR)/riscv-none-elf-ar r scenarios_a3.a *.o

.PHONY: clean
clean:
	rm -f *.a
	rm -f *.o
	rm -f *.bin
	rm -f *.dis

.PHONY: bin
bin: boot-emsa5.elf fw-emsa5.elf
	$(TOOL_DIR)/riscv-none-elf-objcopy -O binary boot-emsa5.elf boot-emsa5.bin
	$(TOOL_DIR)/riscv-none-elf-objcopy -O binary $(TARGET).elf $(TARGET).bin
	$(TOOL_DIR)/riscv-none-elf-objdump -d boot-emsa5.elf > boot-emsa5.dis
	$(TOOL_DIR)/riscv-none-elf-objdump -d $(TARGET).elf > $(TARGET).dis

.PHONY: upload
upload:
	scp *.elf *.bin *.dis ubuntu@kr260:~/TRISTAN/firmware/emsa5

.PHONY: download
download:
	scp ubuntu@kr260:~/TRISTAN/firmware/emsa5/fw-emsa5.ctxp.zst .

.PHONY: analysis
analysis:
	touch fw-emsa5.ctxp.zst
	mv fw-emsa5.ctxp.zst *.elf ../../analysis
