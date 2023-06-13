#include "cpu/exec.h"

void diff_test_skip_qemu();
void diff_test_skip_nemu();

make_EHelper(lidt)
{
  cpu.idtr.limit = vaddr_read(id_dest->addr, 2);
  if (decoding.is_operand_size_16)
  {
    cpu.idtr.base = vaddr_read(id_dest->addr + 2, 3);
  }
  else
  {
    cpu.idtr.base = vaddr_read(id_dest->addr + 2, 4);
  }
  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr)
{
  printf("r2cr\n");
  rtl_mv(&t1, &id_src->val);
  printf("r2cr\n");
  switch (id_dest->reg)
  {
  case 0:
    cpu.cr0.val = t1;
    break;
  case 3:
    cpu.cr3 = t1;
    break;
  default:
    panic("invalid cr number %d\n", id_dest->reg);
  }
  printf("r2cr\n");
  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r)
{
  rtl_mv(&t1, &id_src->val);
  switch (id_dest->reg)
  {
  case 0:
    rtl_mv(&t1, &cpu.cr0.val);
    break;
  case 3:
    rtl_mv(&t1, &cpu.cr3);
    break;
  default:
    panic("invalid cr number %d\n", id_dest->reg);
  }
  operand_write(id_dest, t1);
  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}

make_EHelper(int)
{
  raise_intr(id_dest->val, decoding.seq_eip);

  print_asm("int %s", id_dest->str);

#ifdef DIFF_TEST
  // diff_test_skip_nemu();
#endif
}

make_EHelper(iret)
{
  rtl_pop(&t2);
  // printf("pop retaddr=%x\n",t2);
  rtl_pop(&cpu.cs);
  rtl_pop(&cpu.eflags);
  decoding.jmp_eip = t2;
  decoding.is_jmp = 1;
  print_asm("iret");
}

uint32_t pio_read(ioaddr_t, int);
void pio_write(ioaddr_t, int, uint32_t);

make_EHelper(in)
{
  t2 = pio_read(id_src->val, id_dest->width);
  operand_write(id_dest, &t2);

  print_asm_template2(in);

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}

make_EHelper(out)
{
  pio_write(id_dest->val, id_src->width, id_src->val);

  print_asm_template2(out);

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}
