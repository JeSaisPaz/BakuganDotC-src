// bdc 0x088d71fc GameGimmickItemBoxDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the item box (breakable obstacle) gimmick
   (`GameGimmickItemBoxCtor`, vtables `g_gameGimmickItemBoxVtbl`/`g_gameGimmickItemBoxVtbl2`):
   destroys the collider `+0x174` through its virtual destructor, drops the shared break-effect
   models `g_itemBoxBreakEffect1`/`g_itemBoxBreakEffect2` when the last box goes
   (`g_itemBoxCount` reaches 0), runs `GameGimmickDtor` and frees when `flags & 1`. */

void GameGimmickItemBoxDtor(GameGimmickItemBox *obj, u32 flags)

{
  if (obj != (GameGimmickItemBox *)0x0) {
    (obj->base).base.base.vtable = &g_gameGimmickItemBoxVtbl;
    (obj->base).vtbl2 = (const VtblEntry *)&g_gameGimmickItemBoxVtbl2;
    if ((obj->base).attached != (void *)0x0) {
      CoreNode *node = (CoreNode *)(obj->base).attached;
      const VtblEntry *dtor = &((const VtblEntry *)node->vtable)[1];

      ((void (*)(void *, s32))dtor->fn)((u8 *)node + dtor->delta, 3);
      (obj->base).attached = (void *)0x0;
    }
    g_itemBoxCount = g_itemBoxCount - 1;
    if (g_itemBoxCount == 0) {
      if (g_itemBoxBreakEffect1 != (GfxModel *)0x0) {
        CoreObjectDeferDelete(&g_itemBoxBreakEffect1->base, 0);
        g_itemBoxBreakEffect1 = (GfxModel *)0x0;
      }
      if (g_itemBoxBreakEffect2 != (GfxModel *)0x0) {
        CoreObjectDeferDelete(&g_itemBoxBreakEffect2->base, 0);
        g_itemBoxBreakEffect2 = (GfxModel *)0x0;
      }
    }
    GameGimmickDtor(&obj->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(obj, (char *)0x0, 0);
      MemUnlock();
    }
  }
  return;
}
