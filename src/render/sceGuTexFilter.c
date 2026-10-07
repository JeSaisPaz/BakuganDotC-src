// bdc 0x08a1f248 sceGuTexFilter
#include "bdc.h"

/* Appends one GE command word to the current libgu display list: TFILTER (0xc6). */

void sceGuTexFilter(u32 min, s32 mag)

{
  u32 *cmd = (u32 *)g_guCurrentContext->listCurrent;

  *cmd = ((u32)mag << 8) | min | 0xc6000000;
  g_guCurrentContext->listCurrent = (unsigned char *)(cmd + 1);
}
