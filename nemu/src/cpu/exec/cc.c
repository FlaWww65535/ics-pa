#include "cpu/rtl.h"

/* Condition Code */

void rtl_setcc(rtlreg_t* dest, uint8_t subcode) {
  bool invert = subcode & 0x1;
  enum {
    CC_O, CC_NO, CC_B,  CC_NB,
    CC_E, CC_NE, CC_BE, CC_NBE,
    CC_S, CC_NS, CC_P,  CC_NP,
    CC_L, CC_NL, CC_LE, CC_NLE
  };

  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
  uint32_t cf;
  rtl_get_CF(&cf);
  uint32_t zf;
  rtl_get_ZF(&zf);
  uint32_t of;
  rtl_get_OF(&of);
  uint32_t sf;
  rtl_get_SF(&sf);
  switch (subcode & 0xe) {
    case CC_O:
    *dest = of;
    break;
    case CC_B:
    *dest = cf;
    break;
    case CC_E:
    *dest = zf;
    break;
    case CC_BE:
    *dest = (cf||zf);
    break;
    case CC_S:
    *dest = sf;
    break;
    case CC_L:
    *dest = (sf!=of);
    break;
    case CC_LE:
    *dest = (zf||(sf!=of));
    break;
    default: panic("should not reach here");
    case CC_P: panic("n86 does not have PF");
  }

  if (invert) {
    rtl_xori(dest, dest, 0x1);
  }
}
