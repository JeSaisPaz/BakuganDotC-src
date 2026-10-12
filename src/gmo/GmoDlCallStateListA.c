// bdc 0x089dd738 GmoDlCallStateListA
#include "bdc.h"

/* Writes a GE `BASE` + `CALL` of the static state list `g_gmoStateListA` and, if texture slot
   `slot` (`g_gmoStateTextures``[slot]`) is set, that texture's commands (`GfxTextureWriteCall`).
   Returns the advanced list pointer (`list + 2`, or the texture call's result). */

u32 *GmoDlCallStateListA(u32 *list, s32 slot)

{
  u32 addr;

  addr = PspAddr(g_gmoStateListA); /* GE address of the list */
  list[0] = ((addr >> 0x18) & 0xf) << 0x10 | 0x10000000; /* BASE */
  list[1] = (addr & 0xffffff) | 0xa000000;                /* CALL */
  if (g_gmoStateTextures[slot] != (void *)0x0) {
    return GfxTextureWriteCall(g_gmoStateTextures[slot],list + 2,0);
  }
  return list + 2;
}
