#include "cpu/exec.h"
//data-mov
make_EHelper(mov);
make_EHelper(push);
//control
make_EHelper(call);
make_EHelper(ret);

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


make_EHelper(lea);

make_EHelper(operand_size);
make_EHelper(nop);

make_EHelper(inv);

make_EHelper(nemu_trap);
