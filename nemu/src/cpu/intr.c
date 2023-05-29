#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  rtl_push(&cpu.eflags);
  rtl_push(&cpu.cs);
  rtl_push(&ret_addr);

  vaddr_t Gate_addr =cpu.idtr.base+NO*4;
  int32_t Gate_lo = vaddr_read(Gate_addr,4)&0xffff;  
  int32_t Gate_hi = vaddr_read(Gate_addr+4,4)&0xffff0000; 

  decoding.jmp_eip=Gate_hi|Gate_lo;
  decoding.is_jmp=true;
}

void dev_raise_intr() {
}
