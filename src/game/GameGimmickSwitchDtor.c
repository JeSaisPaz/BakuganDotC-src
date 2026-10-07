// bdc 0x088db850 GameGimmickSwitchDtor
#include "bdc.h"

/* Destructor of the switch gimmick (`GameGimmickSwitchCtor`, vtable `0x08af3734` slot 1,
   secondary `0x08af37d4`): destroys the attached object `+0x174` through its virtual destructor,
   runs `GameGimmickDtor` and frees the object when `flags & 1`. */

static void DestroyVirtual(void *obj)
{
  const VtblEntry *entry = (const VtblEntry *)((CoreNode *)obj)->vtable + 1;

  ((void (*)(void *, int))entry->fn)((char *)obj + entry->delta, 3);
}

void GameGimmickSwitchDtor(GameGimmickSwitch *gimmick, u32 flags)

{
  if (gimmick != (GameGimmickSwitch *)0x0) {
    gimmick->base.base.base.vtable = g_gameGimmickSwitchVtbl;
    gimmick->base.vtbl2 = g_gameGimmickSwitchVtbl2;
    if (gimmick->base.attached != (void *)0x0) {
      DestroyVirtual(gimmick->base.attached);
      gimmick->base.attached = (void *)0x0;
    }
    if (gimmick->pressCollider != (CoreNode *)0x0) {
      DestroyVirtual(gimmick->pressCollider);
      gimmick->pressCollider = (CoreNode *)0x0;
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
