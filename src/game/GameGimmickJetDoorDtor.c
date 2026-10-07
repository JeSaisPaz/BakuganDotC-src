// bdc 0x088d7ac0 GameGimmickJetDoorDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the Marucho-jet cabin door gimmick (`GameGimmickJetDoorCtor`,
   vtables `0x08af31ac`/`0x08af324c`): restores its vtables, runs `GameGimmickDtor` and frees when
   `flags & 1`. */

void GameGimmickJetDoorDtor(GameGimmickJetDoor *obj, u32 flags)

{
  if (obj != (GameGimmickJetDoor *)0x0) {
    (obj->base).base.base.vtable = &g_gameGimmickJetDoorVtbl;
    (obj->base).vtbl2 = (VtblEntry *)&g_gameGimmickJetDoorVtbl2;
    GameGimmickDtor(&obj->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(obj,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

