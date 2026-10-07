// bdc 0x088dad28 GameGimmickSolidDtor
#include "bdc.h"

/* Destructor of the solid gimmick (`GameGimmickSolidCtor`, vtable `0x08af35d4` slot 1, secondary
   `0x08af3674`): destroys the attached object `+0x174` through its virtual destructor, runs
   `GameGimmickDtor` and frees the object when `flags & 1`. */

static void DestroyVirtual(void *obj)
{
  const VtblEntry *entry = (const VtblEntry *)((CoreNode *)obj)->vtable + 1;

  ((void (*)(void *, int))entry->fn)((char *)obj + entry->delta, 3);
}

void GameGimmickSolidDtor(GameGimmick *gimmick, u32 flags)

{
  if (gimmick != (GameGimmick *)0x0) {
    gimmick->base.base.vtable = g_gameGimmickSolidVtbl;
    gimmick->vtbl2 = g_gameGimmickSolidVtbl2;
    if (gimmick->attached != (void *)0x0) {
      DestroyVirtual(gimmick->attached);
      gimmick->attached = (void *)0x0;
    }
    GameGimmickDtor(gimmick,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(gimmick,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}
