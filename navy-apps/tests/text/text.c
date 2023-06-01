#include <stdio.h>
#include <assert.h>

int main()
{
  FILE *fp = fopen("/share/texts/num", "r+");
  assert(fp);

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  assert(size == 5000);
  printf("step 1");
  fseek(fp, 500 * 5, SEEK_SET);
  size = ftell(fp);
  printf("size:%d\n", size);
  int i, n;
  for (i = 500; i < 1000; i++)
  {
    fscanf(fp, "%d", &n);
    assert(n == i + 1);
  }
  printf("step 2");
  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < 500; i++)
  {
    fprintf(fp, "%4d\n", i + 1 + 1000);
  }
  printf("step 3");
  for (i = 500; i < 1000; i++)
  {
    fscanf(fp, "%d", &n);
    assert(n == i + 1);
  }
  printf("step 4");
  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < 500; i++)
  {
    fscanf(fp, "%d", &n);
    assert(n == i + 1 + 1000);
  }

  fclose(fp);

  printf("PASS!!!\n");

  return 0;
}
