// bdc 0x08a10268 GmoImagePlanAddRef
#include "bdc.h"

/* Adds a reference to the plan's block for pool `pool` (one per carved object) and returns `ptr`.
    */

void *GmoImagePlanAddRef(void *plan, int pool, void *ptr)

{
  if ((plan != (void *)0x0) && (ptr != (void *)0x0)) {
    GmoImageBlockAddRef(((void **)plan)[pool]);
  }
  return ptr;
}

