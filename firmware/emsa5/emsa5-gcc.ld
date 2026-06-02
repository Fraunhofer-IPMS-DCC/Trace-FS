OUTPUT_ARCH( "riscv" )
ENTRY(_start)

MEMORY
{
    ITCM(RX)           : ORIGIN = 0x0, LENGTH = 16K
    ITCM_STACK(!RX)    : ORIGIN = 0x4000, LENGTH = 16K
    AXI_INTC(!RX)      : ORIGIN = 0xa1000000, LENGTH = 64K
    AXI_TIMER(!RX)     : ORIGIN = 0xa1010000, LENGTH = 64K
    AXI_I2C(!RX)       : ORIGIN = 0xa1020000, LENGTH = 64K
    AXI_GPIO(!RX)      : ORIGIN = 0xa1030000, LENGTH = 64K
    AXI_BRAM_CODE(RX)  : ORIGIN = 0xa1040000, LENGTH = 16K
    AXI_BRAM_DATA(!RX) : ORIGIN = 0xa1044000, LENGTH = 16K
    AXI_UARTLITE(!RX)  : ORIGIN = 0xa1050000, LENGTH = 64K
}

stack_size = 4096;

SECTIONS
{
    /* trap vector and boot code */
    .boot : AT(ORIGIN(ITCM))
    {
        *(.boot.trapvec)
        *(.boot)
    } > ITCM

    /* results from asm_test */
    .asm_test (NOLOAD) :
    {
        . = ALIGN(256);
        __asm_test_results = .;
        . = ALIGN(2048);
        __asm_test_results_end = .;
    } > ITCM

    /* TEXT section */
    .text : AT(ORIGIN(AXI_BRAM_CODE))
    {
        __code_start = .;
        *(.init)
        . = ALIGN(128);
        *(.trapvec)
        *(.text.init)
        *(.text)
        *(.text.*)
        *(.gnu.linkonce.t*)
    } > AXI_BRAM_CODE

    . = ALIGN(16);

    /* CONST data section */
    .rodata :
    {
        *(.rodata .rodata.*)
        *(.data.rel.ro)
        *(.gnu.linkonce.r*)
    } > AXI_BRAM_CODE

    . = ALIGN(16);

    Stack (NOLOAD) :
    {
        . = ALIGN(16);
        . = . + stack_size - 16;
        . = ALIGN(16);
        _sp = .;
    } > ITCM_STACK

    . = ALIGN(16);

    /* Standard DATA and user defined DATA/BSS/CONST sections */
    .data : AT(ORIGIN(AXI_BRAM_DATA))
    {
        __exc_reason = .;
        *(.exc_data);
        *(.heartbeat);
        . = ALIGN(16);
        __data_start = .;
        * (.data);
        * (.data*);
        *(*.data);
        * (.sdata);
        *(.gnu.linkonce.d*)
        . = ALIGN(4);
        __data_end = .;
    } > AXI_BRAM_DATA
    __data_load = LOADADDR (.data);
    __data_size = __data_end - __data_start;

    /* BSS section */
    .bss (NOLOAD) :
    {
        __bss_start = .;
        * (.bss);
        * (.bss*);
        * (COMMON);
        *(.gnu.linkonce.b*)
        . = ALIGN(4);
        __bss_end = .;
        . = ALIGN(8);
    } > AXI_BRAM_DATA

    /* AXI Interrupt Controller module */
    .axi_intc (NOLOAD) : AT(ORIGIN(AXI_INTC))
    {
        __axi_intc_base = .;
        *(.axi_intc);
    } > AXI_INTC

    /* AXI TIMER module */
    .axi_timer (NOLOAD) : AT(ORIGIN(AXI_TIMER))
    {
        __axi_timer_base = .;
        *(.axi_timer);
    } > AXI_TIMER

    /* AXI I2C module */
    .axi_i2c (NOLOAD) : AT(ORIGIN(AXI_I2C))
    {
        __axi_i2c_base = .;
        *(.axi_i2c);
    } > AXI_I2C

    /* AXI GPIO module */
    .axi_gpio (NOLOAD) : AT(ORIGIN(AXI_GPIO))
    {
        __axi_gpio_base = .;
        *(.axi_gpio);
    } > AXI_GPIO

    /* AXI UART module */
    .axi_uartlite (NOLOAD) : AT(ORIGIN(AXI_UARTLITE))
    {
        __axi_uartlite_base = .;
        *(.axi_uartlite);
    } > AXI_UARTLITE
}
