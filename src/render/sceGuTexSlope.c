// bdc 0x08a1f2cc sceGuTexSlope
#include "bdc.h"

void sceGuTexSlope(float slope)
{
  union { float f; u32 u; } bits;
  u32 *list;

  bits.f = slope;
  list = (u32 *)g_guCurrentContext->listCurrent;
  *list = (bits.u >> 8) | 0xd0000000;
  g_guCurrentContext->listCurrent = (unsigned char *)(list + 1);
}
