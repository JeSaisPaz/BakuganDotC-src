// bdc 0x08a1efc4 sceGuDepthBuffer
#include "bdc.h"

void sceGuDepthBuffer(u32 zbp, u32 zbw)
{
  u32 *cmd = (u32 *)g_guCurrentContext->listCurrent;
  cmd[0] = (((zbp & 0xfffffff) >> 0x18) << 0x10) | 0x9f000000 | zbw;
  g_guCurrentContext->listCurrent = (unsigned char *)(cmd + 2);
  cmd[1] = (zbp & 0xffffff) | 0x9e000000;
  g_guDepthBufWidth = zbw;
  g_guDepthBufPtr = zbp;
}
