#include "common.h"
#include "syscall.h"

int sys_none(uint32_t r){
  return 1;
}

_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);
  switch (a[0]) {
    case 0: SYSCALL_ARG1(r)=sys_none(a[0]);
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
