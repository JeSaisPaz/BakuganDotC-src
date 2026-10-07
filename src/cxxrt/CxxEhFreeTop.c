// bdc 0x08a036f8 CxxEhFreeTop
#include "bdc.h"

/* Frees the most recent exception-stack block (`CxxEhStackPop`) and, when that empties a
   region that has a previous one, makes the previous region current, frees the region's buffer
   if it was heap-allocated (`CxxEhFree`) and pops the region's descriptor block from the
   previous region (`CxxEhStackPop`). */

void CxxEhFreeTop(void)

{
  CxxEhRegion *region;

  CxxEhStackPop();
  region = (CxxEhRegion *)g_cxxEhAllocRegion;
  if (region->used == 0 && region->next != NULL) {
    g_cxxEhAllocRegion = region->next;
    if (region->heap != 0) {
      CxxEhFree(region->base);
    }
    CxxEhStackPop();
  }
}
