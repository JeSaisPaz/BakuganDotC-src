// bdc 0x08a036c8 CxxEhStackPop
#include "bdc.h"

/* Pops the most recent block from the exception-object allocation stack (`g_cxxEhStackTop`) and
   gives its bytes back to the current region. */

void CxxEhStackPop(void)

{
  CxxEhBlock *blk = g_cxxEhStackTop;
  CxxEhRegion *region;

  g_cxxEhStackTop = blk->next;
  region = (CxxEhRegion *)g_cxxEhAllocRegion;
  region->used = region->used - blk->size - 0x10;
}
