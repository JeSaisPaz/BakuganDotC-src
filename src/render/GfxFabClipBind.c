// bdc 0x089f90b8 GfxFabClipBind
#include "bdc.h"

/* Binds a `.fab` clip to its fab (`+0x18`) and `MCLP` chunk (`+0x1c`, data at chunk + 0x10) and
   sets the frame to 1. */

void GfxFabClipBind(GfxFabClip *clip, GfxFab *fab, GfxFabClipDef *chunk)

{
  clip->fab = fab;
  clip->def = chunk;
  clip->data = chunk + 1;
  clip->frame = 1;
}
