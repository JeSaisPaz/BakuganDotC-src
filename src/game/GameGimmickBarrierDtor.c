// bdc 0x088d9bdc GameGimmickBarrierDtor
#include "bdc.h"

/* Destructor of the barrier gimmick (`GameGimmickBarrierCtor`, vtable `0x08af33c4` slot 1,
   secondary `0x08af3464`): destroys the attached object `+0x174` through its virtual destructor,
   runs `GameGimmickDtor` and frees the object when `flags & 1`. */

void GameGimmickBarrierDtor(GameGimmickBarrier *gimmick, u32 flags)

{
  
  if (gimmick != (GameGimmickBarrier *)0x0) {
    (gimmick->base).base.base.vtable = &g_gameGimmickBarrierVtbl;
    (gimmick->base).vtbl2 = (VtblEntry *)&g_gameGimmickBarrierVtbl2;
    if ((gimmick->base).attached != (void *)0x0) {
      CoreNode *node = (CoreNode *)(gimmick->base).attached;
      const VtblEntry *dtor = &((const VtblEntry *)node->vtable)[1];

      ((void (*)(void *, s32))dtor->fn)((u8 *)node + dtor->delta, 3);
      (gimmick->base).attached = (void *)0x0;
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

