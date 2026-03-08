#include <stdio.h>

void assert(int e)
{
#if !defined(NDEBUG)
  if (e == 0)
    printf("Assertion failed\n");
#endif
}
