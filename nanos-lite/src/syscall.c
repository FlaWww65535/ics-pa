#include "common.h"
#include "syscall.h"

int sys_none(uint32_t r){
  return 1;
}

int sys_exit(uint32_t r){
  _halt(r);
}

_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);
  switch (a[0]) {
    case SYS_none: SYSCALL_ARG1(r)=sys_none(a[0]);
    case SYS_exit: SYSCALL_ARG1(r)=sys_exit(a[0]);
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
