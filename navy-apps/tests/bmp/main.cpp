#include <assert.h>
#include <stdlib.h>
#include <ndl.h>
#include <stdio.h>

int main()
{
  NDL_Bitmap *bmp = (NDL_Bitmap *)malloc(sizeof(NDL_Bitmap));
  printf("done\n");
  NDL_LoadBitmap(bmp, "/share/pictures/projectn.bmp");
  printf("done\n");
  assert(bmp->pixels);
  printf("done\n");
  NDL_OpenDisplay(bmp->w, bmp->h);
  printf("done\n");
  NDL_DrawRect(bmp->pixels, 0, 0, bmp->w, bmp->h);
  printf("done\n");
  NDL_Render();
  printf("done\n");
  NDL_CloseDisplay();

  while (1)
    ;
  return 0;
}
