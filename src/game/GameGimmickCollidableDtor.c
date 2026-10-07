// bdc 0x088d505c GameGimmickCollidableDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the breakable container gimmick (`GameGimmickCollidableCtor`,
   vtables `0x08af2e2c`/`0x08af2ecc`): deletes its two colliders `+0x174`/`+0x180` through their
   virtual deleter (slot 1, flag 3), frees the three aligned buffers `+0x184..`, stops its attached
   effect 0x56 when `noEffect` is clear and the event area/block bytes 0 and 2 of
   `g_gameEventFlags` are both 0, then runs `GameGimmickDtor` and frees `obj` when `flags & 1`.
   Does nothing for a NULL `obj`. */

void GameGimmickCollidableDtor(GameGimmickCollidable *obj, u32 flags)
{
  CoreNode *node;
  const VtblEntry *entry;
  int i;

  if (obj == NULL) {
    return;
  }
  obj->base.base.base.vtable = g_gameGimmickCollidableVtbl;
  obj->base.vtbl2 = g_gameGimmickCollidableVtbl2;
  node = (CoreNode *)obj->base.attached;
  if (node != NULL) {
    entry = &((const VtblEntry *)node->vtable)[1];
    ((void (*)(void *, int))entry->fn)((u8 *)node + entry->delta, 3);
    obj->base.attached = NULL;
  }
  node = obj->hitCollider;
  if (node != NULL) {
    entry = &((const VtblEntry *)node->vtable)[1];
    ((void (*)(void *, int))entry->fn)((u8 *)node + entry->delta, 3);
    obj->hitCollider = NULL;
  }
  for (i = 0; i < 3; i++) {
    MemFreeAligned(obj->recordCopy[i]);
    obj->recordCopy[i] = NULL;
  }
  if (obj->noEffect == 0 && g_gameEventFlags[0] == 0 && g_gameEventFlags[2] == 0) {
    GfxEffectStopAttached(g_worldEffectMgr, 0x56, &obj->base.base.data->rootMatrix[12]);
  }
  GameGimmickDtor(&obj->base, 0);
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(obj, NULL, 0);
    MemUnlock();
  }
}
