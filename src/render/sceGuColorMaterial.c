// bdc 0x08a1ed38 sceGuColorMaterial
#include "bdc.h"

/* Appends one GE command word to the current libgu display list: MATERIAL (0x53). */

void sceGuColorMaterial(u32 components)

{
  u32 *cmd = (u32 *)g_guCurrentContext->listCurrent;

  *cmd = (components & 7) | 0x53000000;
  g_guCurrentContext->listCurrent = (unsigned char *)(cmd + 1);
}
