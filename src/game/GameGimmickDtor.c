// bdc 0x088d9284 GameGimmickDtor
#include "bdc.h"

/* Destructor of the gimmick base (`GameGimmickCtor`, vtable `0x08af3314` slot 1): reinstalls the
   gimmick vtables (`+0x14 = 0x08af3314`, `+0x160 = 0x08af33b4`, then the secondary base
   `0x08af6e10`), runs `GfxModelDtor` and frees the object when `flags & 1`. Called by every
   derived gimmick destructor. */

void GameGimmickDtor(GameGimmick *gimmick, u32 flags)

{
  if (gimmick != (GameGimmick *)0x0) {
    (gimmick->base).base.vtable = g_gameGimmickVtbl;
    gimmick->vtbl2 = g_gameGimmickVtbl2;
    /* sub-object cast at +0x140 is never null */
    gimmick->vtbl2 = g_gameGimmickSubBaseVtbl;
    GfxModelDtor(&gimmick->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(gimmick,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

