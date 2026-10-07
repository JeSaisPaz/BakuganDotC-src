// bdc 0x0882c354 UiTalkFreeCutInTextures
#include "bdc.h"

/* Releases the card-art textures of the talk/HUD task (`UiGetTalkTask`) (id 0x6e,
   `BtlHudUpdate`)'s cut-ins: deletes the textures `cutInTexA` and `cutInTexB` (virtual destructor,
   slot at vtable+0xc with this-adjust at vtable+8) and frees their aligned pixel buffers
   `cutInPixelsA` / `cutInPixelsB` (`MemFreeAligned`). Used by
   `BtlHudUpdateAbilityCutIn` and `BtlMainPhaseTalk`. */

typedef struct UiTalkVtable {
  u8 _pad0[8];
  s16 thisAdjust;
  s16 _pad1;
  void (*dtor)(void *self, int flags);
} UiTalkVtable;

void UiTalkFreeCutInTextures(void *win)
{
  BtlHud *hud = (BtlHud *)win;
  CoreObject *obj = hud->cutInTexA;
  if (obj != 0) {
    const UiTalkVtable *vt = (const UiTalkVtable *)obj->vtable;
    vt->dtor((char *)obj + vt->thisAdjust, 3);
    hud->cutInTexA = 0;
  }
  MemFreeAligned(hud->cutInPixelsA);
  obj = hud->cutInTexB;
  hud->cutInPixelsA = 0;
  if (obj != 0) {
    const UiTalkVtable *vt = (const UiTalkVtable *)obj->vtable;
    vt->dtor((char *)obj + vt->thisAdjust, 3);
    hud->cutInTexB = 0;
  }
  MemFreeAligned(hud->cutInPixelsB);
  hud->cutInPixelsB = 0;
}
