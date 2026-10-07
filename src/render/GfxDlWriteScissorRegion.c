// bdc 0x08a1f794 GfxDlWriteScissorRegion
#include "bdc.h"

/* libgu internal scissor writer behind `sceGuScissor`: stores `x`, `y`, `x+w-1`, `y+h-1` in
   the context's scissor fields and, when the scissor test is on, writes SCISSOR1/2 (`0xd4`/`0xd5`)
   and REGION1/2 (`0x15`/`0x16`) into `ctx`'s list. */

void GfxDlWriteScissorRegion(GuContext *ctx, s32 x, s32 y, s32 w, s32 h)
{
  s32 y1 = y + h - 1;
  u32 x1 = (x + w) - 1;
  u32 packed = y1 * 0x400 | x1;
  u32 *p;

  ctx->scissorX0 = x;
  ctx->scissorY0 = y;
  ctx->scissorX1 = x1;
  ctx->scissorY1 = y1;
  if (ctx->scissorEnable != 0) {
    p = (u32 *)ctx->listCurrent;
    *p = y << 10 | x | 0xd4000000;
    ctx->listCurrent = (u8 *)(p + 4);
    p[1] = packed | 0xd5000000;
    p[2] = 0x15000000;
    p[3] = packed | 0x16000000;
  }
}
