// bdc 0x089f168c GfxDlCallSpriteState
#include "bdc.h"

/* Writes a GE `BASE`+`CALL` to the sprite state list `g_gfxSpriteStateList`, initialising its
   patchable words once (texture-function/blend constants `0x63`, `0x64`, `0x65`, `0x5b` and a
   nested `BASE`+`JUMP` to `g_gfxSpriteSubList`). Returns the advanced list pointer. */

u32 *GfxDlCallSpriteState(u32 *list)
{
  u32 sub = PspAddr(&g_gfxSpriteSubList);
  u32 st = PspAddr(g_gfxSpriteStateList);

  if (g_gfxSpriteStateInit == 0) {
    g_gfxSpriteStateInit = 1;
    g_gfxSpriteStateList[3] = 0x63000000;
    g_gfxSpriteStateList[4] = 0x64bf8000;
    g_gfxSpriteStateList[5] = 0x65000000;
    g_gfxSpriteStateList[10] = 0x5b000000;
    g_gfxSpriteStateList[0xf] = (((sub >> 24) & 0xf) << 16) | 0x10000000;
    g_gfxSpriteStateList[0x10] = (sub & 0xffffff) | 0x01000000;
  }
  list[0] = (((st >> 24) & 0xf) << 16) | 0x10000000;
  list[1] = (st & 0xffffff) | 0x0a000000;
  return list + 2;
}
