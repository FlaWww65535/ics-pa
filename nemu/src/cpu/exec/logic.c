#include "cpu/exec.h"

make_EHelper(test) {
  rtl_and(&t2, &id_dest->val, &id_src->val);

  rtl_update_ZFSF(&t2, id_dest->width);

  rtl_set_CF(&tzero);
  rtl_set_OF(&tzero);
  print_asm_template2(test);
}

make_EHelper(and) {
  rtl_and(&t2, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t2);

  rtl_update_ZFSF(&t2, id_dest->width);

  rtl_set_CF(&tzero);
  rtl_set_OF(&tzero);
  print_asm_template2(and);
}

make_EHelper(xor) {
  rtl_xor(&t2, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t2);

  rtl_update_ZFSF(&t2, id_dest->width);

  rtl_set_CF(&tzero);
  rtl_set_OF(&tzero);
  
  print_asm_template2(xor);
}

make_EHelper(or) {
  rtl_or(&t2, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t2);

  rtl_update_ZFSF(&t2, id_dest->width);

  rtl_set_CF(&tzero);
  rtl_set_OF(&tzero);
  print_asm_template2(or);
}

make_EHelper(sar) {
  // unnecessary to update CF and OF in NEMU
  rtl_mv(&t2,&id_dest->val);
  rtl_mv(&t3,&id_src->val);
  rtl_sar(&t0,&t2,&t3);
  operand_write(id_dest,&t0);
  print_asm_template2(sar);
}

make_EHelper(shl) {
  
  // unnecessary to update CF and OF in NEMU
  rtl_mv(&t2,&id_dest->val);
  rtl_mv(&t3,&id_src->val);
  rtl_shl(&t0,&t2,&t3);
  operand_write(id_dest,&t0);
  print_asm_template2(shl);
}

make_EHelper(shr) {
  // unnecessary to update CF and OF in NEMU
  rtl_mv(&t2,&id_dest->val);
  rtl_mv(&t3,&id_src->val);
  rtl_shr(&t0,&t2,&t3);
  operand_write(id_dest,&t0);
  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint8_t subcode = decoding.opcode & 0xf;
  rtl_setcc(&t2, subcode);
  int width = id_dest->width;
  id_dest->width=1;
  operand_write(id_dest, &t2);
  id_dest->width=width;

  print_asm("set%s %s", get_cc_name(subcode), id_dest->str);
}

make_EHelper(not) {
  rtl_mv(&t2,&id_dest->val);
  printf("dest%d ",id_dest->val);
  rtl_not(&t2);
  printf("res%d\n",t2);
  operand_write(id_dest, &t2);
  print_asm_template1(not);
}
