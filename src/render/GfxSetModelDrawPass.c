// bdc 0x089e1054 GfxSetModelDrawPass
#include "bdc.h"

/* Stores its argument in `g_gfxModelDrawPass` (singleton accessor, named by `bdc singleton`). */

void GfxSetModelDrawPass(s32 value)

{
  g_gfxModelDrawPass = value;
  return;
}

