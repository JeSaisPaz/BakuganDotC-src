// bdc 0x088da86c GameGimmickEffectMarkerDtor
#include "bdc.h"

/* Destructor of the effect-marker gimmick (`GameGimmickEffectMarkerCtor`, vtable `0x08af3524`
   slot 1, secondary `0x08af35c4`): destroys the attached object `+0x174` through its virtual
   destructor, runs `GameGimmickDtor` and frees the object when `flags & 1`. */

void GameGimmickEffectMarkerDtor(GameGimmickEffectMarker *gimmick, u32 flags)

{
  
  if (gimmick != (GameGimmickEffectMarker *)0x0) {
    (gimmick->base).base.base.vtable = &g_gameGimmickEffectMarkerVtbl;
    (gimmick->base).vtbl2 = (VtblEntry *)&g_gameGimmickEffectMarkerVtbl2;
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

