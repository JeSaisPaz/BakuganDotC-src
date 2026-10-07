// bdc 0x08a1f690 GfxDlWriteLightColor
#include "bdc.h"

/* Display-list writer for a light colour (game copy of libgu `sceGuLightColor` writing to the
   context's list pointer `ctx->listCurrent`): by `component` (1 ambient, 2 diffuse, 3
   ambient+diffuse, 4 specular, 6 diffuse+specular) emits GE command `0x8f`, `0x90` and/or `0x91` +
   `light * 3` with the 24-bit `color`; other values write nothing. */

void GfxDlWriteLightColor(GuContext *ctx, s32 light, s32 component, u32 color)
{
  s32 cmd;
  u32 *p;

  switch (component) {
  case 1:
    cmd = light * 3 + 0x8f;
    break;
  case 2:
    cmd = light * 3 + 0x90;
    break;
  case 3:
    p = (u32 *)ctx->listCurrent;
    p[0] = (u32)(light * 3 + 0x8f) << 24 | (color & 0xffffff);
    ctx->listCurrent = (u8 *)(p + 2);
    p[1] = (u32)(light * 3 + 0x90) << 24 | (color & 0xffffff);
    return;
  case 4:
    cmd = light * 3 + 0x91;
    break;
  case 6:
    p = (u32 *)ctx->listCurrent;
    p[0] = (u32)(light * 3 + 0x90) << 24 | (color & 0xffffff);
    ctx->listCurrent = (u8 *)(p + 2);
    p[1] = (u32)(light * 3 + 0x91) << 24 | (color & 0xffffff);
    return;
  default:
    return;
  }
  p = (u32 *)ctx->listCurrent;
  *p = (u32)cmd << 24 | (color & 0xffffff);
  ctx->listCurrent = (u8 *)(p + 1);
}
