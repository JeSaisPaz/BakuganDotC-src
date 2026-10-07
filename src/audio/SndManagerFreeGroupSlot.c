// bdc 0x089c5fa8 SndManagerFreeGroupSlot
#include "bdc.h"

/* Tries to release group slot `slot` of the `SndManager` under the manager lock. Frees it only
   when it holds a bank (`bankId != -1`), is idle (`state == 0`), none of its sounds is playing
   (`SndManagerIsGroupPlaying`) and the Sony sound layer accepts the unregister
   (`SndSsBankUnregister(bankId) == 0`): then `bankId = -1`, destroys the bank package through its
   vtable (entry 1, flag 2), clears `name`, `dataSize`, `loaded`, sets `groupId = -1`, `data = 1`,
   drops the data manager references held by the slot (`IoGetDataMng` /
   `IoDataMngReleaseOwner`), clears `fileReq[3]` and writes `0x80 | (groupId & 0xff)` — by then
   always 0xff — to the GPO LED port with `sceKernelSetGPO`. Returns true when the slot ends up
   with no bank (`bankId == -1`), also when it had none to begin with; false otherwise. */

bool SndManagerFreeGroupSlot(SndManager *mgr, s32 slot)
{
  SndGroupSlot *group;
  bool freed;

  CoreLockAcquire(mgr->lock);
  group = &mgr->groups[slot];
  if (group->bankId != -1 && group->state == 0 &&
      !SndManagerIsGroupPlaying(mgr, (u32)group->groupId) &&
      SndSsBankUnregister((u32)group->bankId) == 0) {
    IoLzsPackage *bank = group->bank;
    const VtblEntry *dtor;

    group->bankId = -1;
    dtor = (const VtblEntry *)bank->base.vtable + 1;
    ((void (*)(void *, s32))dtor->fn)((u8 *)bank + dtor->delta, 2);
    group->name = NULL;
    group->dataSize = 0;
    group->loaded = 0;
    group->groupId = -1;
    group->data = (void *)(uintptr_t)1;
    IoDataMngReleaseOwner(IoGetDataMng(), group);
    group->fileReq[0] = 0;
    group->fileReq[1] = 0;
    group->fileReq[2] = 0;
    sceKernelSetGPO(((u32)group->groupId & 0xff) | 0x80);
  }
  freed = group->bankId == -1;
  CoreLockRelease(mgr->lock);
  return freed;
}
