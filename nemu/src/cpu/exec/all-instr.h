#include "cpu/exec.h"
// data-mov
make_EHelper(mov);
make_EHelper(movzx);
make_EHelper(movsx);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(pusha);
make_EHelper(popa);
make_EHelper(cwtl);
make_EHelper(cltd);
// system
make_EHelper(lidt);
make_EHelper(int);
make_EHelper(iret);
make_EHelper(in);
make_EHelper(out);
make_EHelper(mov_r2cr);
make_EHelper(mov_cr2r);
// control
make_EHelper(call);
make_EHelper(call_rm);
make_EHelper(ret);
make_EHelper(jcc);
make_EHelper(jmp);
make_EHelper(jmp_rm);
make_EHelper(leave);

// arith/logic
make_EHelper(add);
make_EHelper(or);
make_EHelper(adc);
make_EHelper(sbb);
make_EHelper(and);
make_EHelper(sub);
make_EHelper(xor);
make_EHelper(cmp);

make_EHelper(setcc);
make_EHelper(test);
make_EHelper(inc);
make_EHelper(dec);
make_EHelper(neg);

make_EHelper(idiv);
make_EHelper(div);
make_EHelper(imul1);
make_EHelper(imul2);
make_EHelper(mul);

make_EHelper(shl);
make_EHelper(shr);
make_EHelper(sar);

make_EHelper(not );
make_EHelper(lea);

make_EHelper(operand_size);
make_EHelper(nop);

make_EHelper(inv);

make_EHelper(nemu_trap);
