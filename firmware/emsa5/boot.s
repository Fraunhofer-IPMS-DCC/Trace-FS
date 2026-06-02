    .section .boot.trapvec, "ax"
    .globl _trap_vector_table_boot

_trap_vector_table:
    j _synchronous_exception
    j _unhandled_trap
    j _unhandled_trap
    j _software_interrupt
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _machine_timer_interrupt
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _external_interrupt
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _unhandled_trap
    j _non_maskable_interrupt

    .section .boot, "ax"
    .globl _boot
    .type _boot,@function

_boot:
    la t1, __code_start
    jr t1

.weak _unhandled_trap
_unhandled_trap:
    j _unhandled_trap

.weak _synchronous_exception
_synchronous_exception:
    j _synchronous_exception

.weak _software_interrupt
_software_interrupt:
    j _software_interrupt

.weak _machine_timer_interrupt
_machine_timer_interrupt:
    j _machine_timer_interrupt

.weak _external_interrupt
_external_interrupt:
    j _external_interrupt

.weak _non_maskable_interrupt
_non_maskable_interrupt:
    j _non_maskable_interrupt
