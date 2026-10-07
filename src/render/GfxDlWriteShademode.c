// bdc 0x08a1f814 GfxDlWriteShademode
#include "bdc.h"

/* libgu internal shade-model writer behind `sceGuShadeModel`: writes SHADE (`0x50`) with 1
   (`GU_SMOOTH`) for non-zero `mode`, else 0 (`GU_FLAT`), into `ctx`'s list. */

void GfxDlWriteShademode(GuContext *ctx, s32 mode)

{
  u32 *p = (u32 *)ctx->listCurrent;

  *p = mode != 0 ? 0x50000001 : 0x50000000;
  ctx->listCurrent = (unsigned char *)(p + 1);
}
