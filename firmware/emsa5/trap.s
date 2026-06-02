    .section .trapvec, "ax"
    .globl _trap_vector_table

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

.weak _unhandled_trap
_unhandled_trap:
    j _unhandled_trap

.weak _synchronous_exception
_synchronous_exception:
    j trap_entry
_synchronous_exception_loop:
    j _synchronous_exception_loop

.weak _software_interrupt
_software_interrupt:
    j trap_entry
_software_interrupt_loop:
    j _software_interrupt_loop

.weak _machine_timer_interrupt
_machine_timer_interrupt:
    j trap_entry
_machine_timer_interrupt_loop:
    j _machine_timer_interrupt_loop

.weak _external_interrupt
_external_interrupt:
    j trap_entry
_external_interrupt_loop:
    j _external_interrupt_loop

.weak _non_maskable_interrupt
_non_maskable_interrupt:
    j _non_maskable_interrupt

/*
 * borrowed from TGC-FW
 */
  .align 7
  .global trap_entry
trap_entry:
  addi sp, sp, -16*4
  sw x1, 1*4(sp)
  sw x5, 2*4(sp)
  sw x6, 3*4(sp)
  sw x7, 4*4(sp)
  sw x10, 5*4(sp)
  sw x11, 6*4(sp)
  sw x12, 7*4(sp)
  sw x13, 8*4(sp)
  sw x14, 9*4(sp)
  sw x15, 10*4(sp)
  sw x16, 11*4(sp)
  sw x17, 12*4(sp)
  sw x28, 13*4(sp)
  sw x29, 14*4(sp)
  sw x30, 15*4(sp)
  sw x31, 16*4(sp)

  csrr a0, mcause
  csrr a1, mepc

  la a4, __exc_reason
  sw a0, +0(a4)
  sw a1, +4(a4)

  call handle_trap

  csrw mepc, a0

  lw x1, 1*4(sp)
  lw x5, 2*4(sp)
  lw x6, 3*4(sp)
  lw x7, 4*4(sp)
  lw x10, 5*4(sp)
  lw x11, 6*4(sp)
  lw x12, 7*4(sp)
  lw x13, 8*4(sp)
  lw x14, 9*4(sp)
  lw x15, 10*4(sp)
  lw x16, 11*4(sp)
  lw x17, 12*4(sp)
  lw x28, 13*4(sp)
  lw x29, 14*4(sp)
  lw x30, 15*4(sp)
  lw x31, 16*4(sp)
  addi sp, sp, +16*4
  mret
