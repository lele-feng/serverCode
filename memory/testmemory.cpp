#include <iostream>
#include <stdio.h>

void dump(const char* filename)
{
  char buf[65536];
  FILE* fp = fopen(filename, "r");
  if (fp)
  {
    ssize_t nr;
    while ( (nr = fread(buf, 1, sizeof buf, fp)) > 0)
      fwrite(buf, 1, nr, stdout);

    fclose(fp);
  }
}

void snapshot(const char* name)
{
  printf("===== %s =====\n", name);
  printf("---/proc/meminfo:\n");
  dump("/proc/meminfo");
  printf("---/proc/slabinfo:\n");
  dump("/proc/slabinfo");
}

int main()
{
  snapshot("fe");
  return 0;
}
