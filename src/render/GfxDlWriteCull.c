// bdc 0x08a1f664 GfxDlWriteCull
#include "bdc.h"

/* libgu internal front-face writer behind `sceGuFrontFace`: writes FFACE/cull order (`0x9b`) with
   0 for non-zero `order` (`GU_CW`) and 1 for 0 (`GU_CCW`) into `ctx`'s list. */

void GfxDlWriteCull(GuContext *ctx, s32 order)

{
  u32 *p = (u32 *)ctx->listCurrent;

  *p = order != 0 ? 0x9b000000 : 0x9b000001;
  ctx->listCurrent = (unsigned char *)(p + 1);
}
