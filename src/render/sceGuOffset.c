// bdc 0x08a1f1bc sceGuOffset
#include "bdc.h"

void sceGuOffset(u32 x, u32 y)
{
  GuContext *ctx = g_guCurrentContext;
  u32 *list = (u32 *)ctx->listCurrent;

  list[0] = (x << 4) | 0x4c000000;
  ctx->listCurrent = (unsigned char *)(list + 2);
  list[1] = (y << 4) | 0x4d000000;
}
