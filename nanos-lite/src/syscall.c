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

int sys_brk(uintptr_t addr)
{
  return mm_brk(addr);
}

int sys_open(uintptr_t filename, uintptr_t flags, uintptr_t mode)
{
  return fs_open(filename, flags, mode);
}

int sys_close(uintptr_t fd)
{
  return fs_close(fd);
}

int sys_read(uintptr_t fd, uintptr_t buf, uintptr_t len)
{
  return fs_read(fd, buf, len);
}

int sys_write(uintptr_t fd, uintptr_t buf, uintptr_t len)
{
  // Log("_write\n");
  return fs_write(fd, buf, len);
}

int sys_lseek(uintptr_t fd, uintptr_t offset, uintptr_t whence)
{
  // Log("_lseek\n");
  return fs_lseek(fd, offset, whence);
}

_RegSet *do_syscall(_RegSet *r)
{
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);
  a[1] = SYSCALL_ARG2(r);
  a[2] = SYSCALL_ARG3(r);
  a[3] = SYSCALL_ARG4(r);
  Log("do_syscall: No %d\n", a[0]);
  switch (a[0])
  {
  case SYS_none:
    SYSCALL_ARG1(r) = sys_none();
    break;
  case SYS_exit:
    SYSCALL_ARG1(r) = sys_exit(a[1]);
    break;
  case SYS_brk:
    SYSCALL_ARG1(r) = sys_brk(a[1]);
    break;
  case SYS_open:
    SYSCALL_ARG1(r) = sys_open(a[1], a[2], a[3]);
    break;
  case SYS_close:
    SYSCALL_ARG1(r) = sys_close(a[1]);
    break;
  case SYS_read:
    SYSCALL_ARG1(r) = sys_read(a[1], a[2], a[3]);
    break;
  case SYS_write:
    SYSCALL_ARG1(r) = sys_write(a[1], a[2], a[3]);
    break;
  case SYS_lseek:
    SYSCALL_ARG1(r) = sys_lseek(a[1], a[2], a[3]);
    break;
  default:
    panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
