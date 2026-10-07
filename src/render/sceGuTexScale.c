// bdc 0x08a1ed8c sceGuTexScale
#include "bdc.h"

void sceGuTexScale(float u, float v)
{
  u32 *cmd = (u32 *)g_guCurrentContext->listCurrent;
  union { float f; u32 u; } uu, vv;
  uu.f = u;
  vv.f = v;
  cmd[0] = (uu.u >> 8) | 0x48000000;
  g_guCurrentContext->listCurrent = (unsigned char *)(cmd + 2);
  cmd[1] = (vv.u >> 8) | 0x49000000;
}
