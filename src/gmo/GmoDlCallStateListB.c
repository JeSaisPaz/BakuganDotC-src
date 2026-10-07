// bdc 0x089dd7ac GmoDlCallStateListB
#include "bdc.h"

/* Writes a GE `CALL` to the static state list at `0x08aa3058` and, if texture slot `slot`
   (`g_gmoStateTextures``[slot]`) is set, that texture's commands (`GfxTextureWriteCall`).
   Returns the advanced list pointer (`list + 2`, or the texture call's result). */

u32 *GmoDlCallStateListB(u32 *list, s32 slot)

{
  *list = 0x10080000;
  list[1] = 0xaaa3058;
  if (g_gmoStateTextures[slot] != (void *)0x0) {
    return GfxTextureWriteCall(g_gmoStateTextures[slot],list + 2,0);
  }
  return list + 2;
}
