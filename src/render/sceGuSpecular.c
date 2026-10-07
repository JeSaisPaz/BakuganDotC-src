// bdc 0x08a1ed60 sceGuSpecular
#include "bdc.h"

/* Appends one GE command word to the current libgu display list: SPOW (0x5b), float bits >> 8. */

void sceGuSpecular(float power)

{
  u32 *cmd = (u32 *)g_guCurrentContext->listCurrent;

  *cmd = (*(u32 *)&power >> 8) | 0x5b000000;
  g_guCurrentContext->listCurrent = (unsigned char *)(cmd + 1);
}
