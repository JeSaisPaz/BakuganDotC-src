// bdc 0x08a2df5c SndEmitterListHead
#include "bdc.h"

/* Returns the sentinel node of the live chain (`list+0`); the first real node is its `next`
   (`SndEmitterNodeGetNext`). */

CorePrioNode *SndEmitterListHead(CorePrioList *list)

{
  return list->active;
}

