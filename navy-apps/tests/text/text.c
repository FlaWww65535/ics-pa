#include <stdio.h>
#include <assert.h>

int main() {

  FILE *fp = fopen("/share/texts/num", "r+");
  assert(fp);
  
  printf("text begin\n");
  fseek(fp, 0, SEEK_END);
  long size = fseek(fp,0,SEEK_CUR);
  printf("size = %d\n",size);
  assert(size == 5000);


  fseek(fp, 500 * 5, SEEK_SET);
  int i, n;
  for (i = 500; i < 1000; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1);
  }
  printf("text para 2 \n");

  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < 500; i ++) {
    fprintf(fp, "%4d\n", i + 1 + 1000);
  }

  printf("text para 3 \n");

  for (i = 500; i < 1000; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1);
  }

  printf("text para 4 \n");

  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < 500; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1 + 1000);
  }
  printf("text para 5 \n");

  fclose(fp);

  printf("PASS!!!\n");

  return 0;
}
