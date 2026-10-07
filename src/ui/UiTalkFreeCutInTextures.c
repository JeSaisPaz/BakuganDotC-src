// bdc 0x0882c354 UiTalkFreeCutInTextures
#include "bdc.h"

/* Releases the card-art textures of the talk/HUD task (`UiGetTalkTask`) (id 0x6e,
   `BtlHudUpdate`)'s cut-ins: deletes the textures `cutInTexA` and `cutInTexB` (virtual destructor,
   slot at vtable+0xc with this-adjust at vtable+8) and frees their aligned pixel buffers
   `cutInPixelsA` / `cutInPixelsB` (`MemFreeAligned`). Used by
   `BtlHudUpdateAbilityCutIn` and `BtlMainPhaseTalk`. */

void UiTalkFreeCutInTextures(void *win)
{
  BtlHud *hud = (BtlHud *)win;
  CoreObject *obj = hud->cutInTexA;

  if (obj != NULL) {
    const VtblEntry *dtor = &((const VtblEntry *)obj->vtable)[1];

    ((void (*)(void *, s32))dtor->fn)((u8 *)obj + dtor->delta, 3);
    hud->cutInTexA = NULL;
  }
  MemFreeAligned(hud->cutInPixelsA);
  obj = hud->cutInTexB;
  hud->cutInPixelsA = NULL;
  if (obj != NULL) {
    const VtblEntry *dtor = &((const VtblEntry *)obj->vtable)[1];

    ((void (*)(void *, s32))dtor->fn)((u8 *)obj + dtor->delta, 3);
    hud->cutInTexB = NULL;
  }
  MemFreeAligned(hud->cutInPixelsB);
  hud->cutInPixelsB = NULL;
}
