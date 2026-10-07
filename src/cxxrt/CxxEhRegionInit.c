// bdc 0x08a034d4 CxxEhRegionInit
#include "bdc.h"

/* Zeroes an exception-memory region descriptor (`{next, base, size, used, u8 heap}`). */

void CxxEhRegionInit(void *region)

{
  CxxEhRegion *r = (CxxEhRegion *)region;

  r->next = NULL;
  r->base = NULL;
  r->size = 0;
  r->used = 0;
  r->heap = 0;
}
