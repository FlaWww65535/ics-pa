#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  if(id_src->type==OP_TYPE_IMM && id_src->width==1){
    rtl_sext(&t2,&id_src->val,1);
  }
  rtl_push(&id_dest->val);
  //printf("%d",*id_dest);

  print_asm_template1(push);
}

make_EHelper(pop) {
  rtl_pop(&t2);
  operand_write(id_dest,&t2);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  rtl_mv(&reg_l(R_ESP),&reg_l(R_EBP));
  rtl_pop(&reg_l(R_EBP));
  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    if(reg_w(R_AX)&0x8000){reg_w(R_DX)=0xffff;}
    else{reg_w(R_DX)=0x0000;}
  }
  else {
    if(reg_w(R_EAX)&0x80000000){reg_w(R_EDX)=0xffffffff;}
    else{reg_w(R_EDX)=0x00000000;}
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    rtl_sext(&t2,reg_b(R_AL),1);
    return;
    rtl_msb(&t2, &t2,2);
    rtl_mv(&reg_w(R_AX),&t2);
  }
  else {
    rtl_sext(&t2,reg_w(R_AX),2);
    rtl_msb(&t2, &t2,4);
    rtl_mv(&reg_l(R_EAX),&t2);
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t2, &id_src->val, id_src->width);
  operand_write(id_dest, &t2);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  rtl_li(&t2, id_src->addr);
  operand_write(id_dest, &t2);
  print_asm_template2(lea);
}
