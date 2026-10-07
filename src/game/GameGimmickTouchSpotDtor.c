// bdc 0x088db144 GameGimmickTouchSpotDtor
#include "bdc.h"

/* Destructor of the touch-spot gimmick (`GameGimmickTouchSpotCtor`, vtable `0x08af3684` slot 1,
   secondary `0x08af3724`): destroys the attached object `+0x174` through its virtual destructor,
   runs `GameGimmickDtor` and frees the object when `flags & 1`. */

static void DestroyVirtual(void *obj)
{
  const VtblEntry *entry = (const VtblEntry *)((CoreNode *)obj)->vtable + 1;

  ((void (*)(void *, int))entry->fn)((char *)obj + entry->delta, 3);
}

void GameGimmickTouchSpotDtor(GameGimmickTouchSpot *gimmick, u32 flags)

{
  if (gimmick != (GameGimmickTouchSpot *)0x0) {
    gimmick->base.base.base.vtable = g_gameGimmickTouchSpotVtbl;
    gimmick->base.vtbl2 = g_gameGimmickTouchSpotVtbl2;
    if (gimmick->base.attached != (void *)0x0) {
      DestroyVirtual(gimmick->base.attached);
      gimmick->base.attached = (void *)0x0;
    }
    GameGimmickDtor(&gimmick->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(gimmick,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}
