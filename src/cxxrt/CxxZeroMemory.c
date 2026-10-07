// bdc 0x08a02870 CxxZeroMemory
#include "bdc.h"

/* Thin wrapper: `memset(ptr, 0, size)`. */

void CxxZeroMemory(void *ptr, size_t size)

{
  memset(ptr,0,size);
  return;
}

