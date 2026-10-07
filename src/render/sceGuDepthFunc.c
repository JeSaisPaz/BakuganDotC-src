// bdc 0x08a1f2f8 sceGuDepthFunc
#include "bdc.h"

/* Appends one GE command word to the current libgu display list: ZTST (0xde). */

void sceGuDepthFunc(u32 function)

{
  u32 *cmd = (u32 *)g_guCurrentContext->listCurrent;

  *cmd = function | 0xde000000;
  g_guCurrentContext->listCurrent = (unsigned char *)(cmd + 1);
}
