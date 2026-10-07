// bdc 0x088d62c4 GameGimmickCollectionBoxDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the collection box gimmick (`GameGimmickCollectionBoxCtor`,
   vtables `0x08af2f8c`/`0x08af302c`): `GameGimmickDtor`, frees when `flags & 1`. */

void GameGimmickCollectionBoxDtor(GameGimmickCollectionBox *obj, u32 flags)

{
  if (obj != (GameGimmickCollectionBox *)0x0) {
    (obj->base).base.base.vtable = &g_gameGimmickCollectionBoxVtbl;
    (obj->base).vtbl2 = (VtblEntry *)&g_gameGimmickCollectionBoxVtbl2;
    GameGimmickDtor(&obj->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(obj,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

