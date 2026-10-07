// bdc 0x0889fab8 GameGimmickCorePointDtor
#include "bdc.h"

/* Destructor of the core-point gimmick (`GameGimmickCorePointCtor`, vtable `0x08af234c` slot 1):
   reinstalls the two vtables (`+0x14`, `+0x160`), destroys the attached object `+0x174` through its
   virtual destructor, stops the pickup effect 0xe when `+0x2c1` is set, frees the aligned buffer
   `+0x2d4` (`MemFreeAligned`), destroys the embedded `CoreBezierCtor` curve at `+0x200`
   (`CoreBezierDtor`), runs the gimmick base destructor `GameGimmickDtor` and frees the object
   when bit 0 of `flags` is set. */

void GameGimmickCorePointDtor(GameGimmickCorePoint *obj, u32 flags)

{
  if (obj != (GameGimmickCorePoint *)0x0) {
    (obj->base).base.base.vtable = (void *)&g_gameGimmickCorePointVtbl;
    (obj->base).vtbl2 = (const VtblEntry *)&g_gameGimmickCorePointVtbl2;
    if ((obj->base).attached != (void *)0x0) {
      CoreNode *node = (CoreNode *)(obj->base).attached;
      const VtblEntry *dtor = &((const VtblEntry *)node->vtable)[1];

      ((void (*)(void *, s32))dtor->fn)((u8 *)node + dtor->delta, 3);
      (obj->base).attached = (void *)0x0;
    }
    if (obj->effectAttached != '\0') {
      GfxEffectStopAttached(g_worldEffectMgr,0xe,((obj->base).base.data)->rootMatrix + 0xc);
      obj->effectAttached = '\0';
    }
    if (obj->buffer != (void *)0x0) {
      MemFreeAligned(obj->buffer);
      obj->buffer = (void *)0x0;
    }
    CoreBezierDtor(&obj->hop,2);
    GameGimmickDtor(&obj->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(obj,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}
