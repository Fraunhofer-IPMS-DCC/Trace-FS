    .section .init
    .globl _start
    .type _start,@function
    .type _exit,@function

_start:
    /* initialize stack pointer */
    la sp, _sp

    /* initialize trap vector base address, enable vectored mode */
    la a4, _trap_vector_table
    addi a4, a4, 1
    csrw mtvec, a4

    /* clear reason for exception (mcause, mepc) */
    la a4, __exc_reason
    li a5, 0
    sw a5, (a4)
    sw a5, +4(a4)

    /* argc = argv = 0 */
    li a0, 0
    li a1, 0
    call main

_exit:
    j _exit
