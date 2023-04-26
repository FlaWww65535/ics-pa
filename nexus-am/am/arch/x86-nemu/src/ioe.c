#include <am.h>
#include <x86.h>

#define RTC_PORT 0x48   // Note that this is not standard
static unsigned long boot_time;

void _ioe_init() {
  boot_time = inl(RTC_PORT);
}

unsigned long _uptime() {
  return inl(RTC_PORT)-boot_time;
}

uint32_t* const fb = (uint32_t *)0x40000;

_Screen _screen = {
  .width  = 400,
  .height = 300,
};

extern void* memcpy(void *, const void *, int);

void _draw_rect(const uint32_t *pixels, int x, int y, int w, int h) {
  int i;
  int cnt=0;
  for (i = 0; i < _screen.width * _screen.height; i++) {
    int now_x=i%_screen.width;
    int now_y=i/_screen.width;
    fb[i]=0;
    if(now_x>=x&&now_x<=x+w&&now_y>=y&&now_y<=y+h){
      if(cnt<=w*h)fb[i] = pixels[cnt++];
    }
  }
}

void _draw_sync() {
}

int _read_key() {
  if(inb(0x64))
  {
    return inl(0x60);
  }
  return _KEY_NONE;
}
