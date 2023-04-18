#include "cpu/exec.h"
//data-mov
make_EHelper(mov);
make_EHelper(movzx);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(cwtl);
make_EHelper(cltd);
//system
make_EHelper(in);
make_EHelper(out);
//control
make_EHelper(call);
make_EHelper(ret);
make_EHelper(jcc);
make_EHelper(jmp);

//arith/logic
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
make_EHelper(imul);
make_EHelper(mul);
make_EHelper(not);

make_EHelper(lea);

make_EHelper(operand_size);
make_EHelper(nop);

make_EHelper(inv);

make_EHelper(nemu_trap);
