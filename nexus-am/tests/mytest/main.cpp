#include <am.h>
#include <klib.h>
#define FPS 32

unsigned int canvas[32][32]={255};

int main() {
  _ioe_init();

  unsigned long last = 0;

  while (true) {
    unsigned long upt = _uptime();
    if (upt - last > 1000 / FPS) {
      _draw_rect(&canvas[0][0],5,5,32,32);
      last = upt;
    }
  }

  return 0;
}
