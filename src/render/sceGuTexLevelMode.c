// bdc 0x08a1f274 sceGuTexLevelMode
#include "bdc.h"

void sceGuTexLevelMode(u32 mode, float bias)
{
  u32 *cmd = (u32 *)g_guCurrentContext->listCurrent;
  s32 level = (s32)(bias * g_guTexLevelScale);

  g_guCurrentContext->listCurrent = (u8 *)(cmd + 1);
  if (level < -0x80) {
    level = -0x80;
  }
  if (level > 0x7f) {
    level = 0x7f;
  }
  *cmd = ((u32)level & 0xff) << 0x10 | mode | 0xc8000000;
}
