#include "common.h"

static _RegSet *do_event(_Event e, _RegSet *r)
{
  // Log("do_event: case:%d\n", e.event);
  switch (e.event)
  {
  case _EVENT_SYSCALL:
    do_syscall(r);
    return schedule(r);
    break;
  case _EVENT_TRAP:
    return schedule(r);
    break;
  default:
    panic("Unhandled event ID = %d", e.event);
  }

  return NULL;
}

void init_irq(void)
{
  _asye_init(do_event);
}
