// bdc 0x08a1f358 sceGuBlendFunc
#include "bdc.h"

void sceGuBlendFunc(s32 op, s32 src, s32 dest, u32 srcfix, u32 destfix)

{
  GuContext *ctx;
  u32 *cmd;
  
  ctx = g_guCurrentContext;
  cmd = (u32 *)g_guCurrentContext->listCurrent;
  *cmd = op << 8 | dest << 4 | src | 0xdf000000;
  ctx->listCurrent = (u8 *)(cmd + 3);
  cmd[1] = srcfix & 0xffffff | 0xe0000000;
  cmd[2] = destfix & 0xffffff | 0xe1000000;
  return;
}

