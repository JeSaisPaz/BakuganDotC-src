// bdc 0x088d5e50 GameGimmickSteamDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the steam gimmick (`GameGimmickSteamCtor`, vtables
   `0x08af2edc`/`0x08af2f7c`): deletes the collider `+0x174`, runs `GameGimmickDtor` and frees
   when `flags & 1`. */

void GameGimmickSteamDtor(GameGimmickSteam *obj, u32 flags)

{
  
  if (obj != (GameGimmickSteam *)0x0) {
    (obj->base).base.base.vtable = &g_gameGimmickSteamVtbl;
    (obj->base).vtbl2 = (VtblEntry *)&g_gameGimmickSteamVtbl2;
    if ((obj->base).attached != (void *)0x0) {
      CoreNode *node = (CoreNode *)(obj->base).attached;
      const VtblEntry *dtor = &((const VtblEntry *)node->vtable)[1];

      ((void (*)(void *, s32))dtor->fn)((u8 *)node + dtor->delta, 3);
      (obj->base).attached = (void *)0x0;
    }
    GameGimmickDtor(&obj->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(obj,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

