// bdc 0x0882afbc GfxPlayerBlurWriteTexState
#include "bdc.h"

/* Writes the common GE state for the player blur task (`GfxPlayerBlurTaskCtor`)'s screen-strip
   passes at `dl`: texture mapping on, alpha/depth/stencil tests off, blending on, texture scale
   1/offset 0, then binds `tex` (`GfxTextureWriteCall`). Returns the advanced command pointer (in
   `v0`). */

u32 * GfxPlayerBlurWriteTexState(void *task, u32 *dl, void *tex)

{
  *dl = 0x1e000001;
  dl[1] = 0x22000000;
  dl[2] = 0x23000000;
  dl[3] = 0x24000000;
  dl[4] = 0x21000001;
  dl[5] = 0x483f8000;
  dl[6] = 0x493f8000;
  dl[7] = 0x4a000000;
  dl[8] = 0x4b000000;
  return GfxTextureWriteCall(tex, dl + 9, 0);
}

