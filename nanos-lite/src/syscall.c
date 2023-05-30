#include "common.h"
#include "syscall.h"

int sys_none()
{
  return 1;
}

int sys_exit(uintptr_t r)
{
  _halt(r);
}

int sys_write(uintptr_t fd, uintptr_t buf, uintptr_t cnt)
{
  Log("_write\n");
  switch (fd)
  {
  case 1:
  case 2:
    for (int i = 0; i < cnt; i++)
    {
      _putc(((char *)buf)[i]);
    }
    return cnt;
  default:
    return -1;
  }
}

int sys_brk(uintptr_t addr){
  return 0;
}

_RegSet *do_syscall(_RegSet *r)
{
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);
  a[1] = SYSCALL_ARG2(r);
  a[2] = SYSCALL_ARG3(r);
  a[3] = SYSCALL_ARG4(r);
  switch (a[0])
  {
  case SYS_none:
    SYSCALL_ARG1(r) = sys_none();
    break;
  case SYS_exit:
    SYSCALL_ARG1(r) = sys_exit(a[1]);
    break;
  case SYS_write:
    SYSCALL_ARG1(r) = sys_write(a[1], a[2], a[3]);
    break;
  case SYS_brk:
    SYSCALL_ARG1(r) = sys_brk(a[1]);
    break;
  default:
    panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
