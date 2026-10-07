// bdc 0x08a1ed0c sceGuPatchDivide
#include "bdc.h"

/* Appends one GE command word to the current libgu display list: PSUB/PATCH divide (0x36). */

void sceGuPatchDivide(u32 ulevel, s32 vlevel)

{
  u32 *cmd = (u32 *)g_guCurrentContext->listCurrent;

  *cmd = ((u32)vlevel << 8) | ulevel | 0x36000000;
  g_guCurrentContext->listCurrent = (unsigned char *)(cmd + 1);
}
