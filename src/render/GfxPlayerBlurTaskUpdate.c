// bdc 0x0882afa0 GfxPlayerBlurTaskUpdate
#include "bdc.h"

/* Update of the player blur task (`GfxPlayerBlurTaskCtor`) (vtable slot 2): calls
   `GfxPlayerBlurTaskStep`. */

void GfxPlayerBlurTaskUpdate(void *task)

{
  GfxPlayerBlurTaskStep(task);
  return;
}

