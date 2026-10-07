// bdc 0x089c6310 SndManagerPlay
#include "bdc.h"

/* Queues a sound request on the `SndManager`: under the manager lock, when the manager is not
   blocked (`flag8be5 == 0`) and running (`state == 5`), takes a free command slot
   (`SndManagerFindFreeSlot`), bumps `handleCounter` (wrapping past 0x7ffffff to 0x10000), stores
   `soundId` in `cmdId[slot]` and the handle `handleCounter | (category & 7) << 28 | (flag & 1) << 27`
   in `cmdHandle[slot]`. Category 0 is derived from the id: masked id in 0x4200000..0x5200003 -> 4,
   id in `g_sndCategory1SoundIds` -> 1, else 2. Returns the handle, or 0 when no request was
   queued. */

u32 SndManagerPlay(SndManager *mgr, u32 soundId, u32 category, u32 flag)
{
  u32 handle = 0;
  s32 slot = -1;
  CoreLock *lock;

  CoreLockAcquire(mgr->lock);
  if (mgr->flag8be5 == 0 && mgr->state == 5) {
    slot = SndManagerFindFreeSlot(mgr);
  }
  lock = mgr->lock;
  if (slot >= 0) {
    mgr->handleCounter = mgr->handleCounter + 1;
    if (mgr->handleCounter > 0x7ffffffu) {
      mgr->handleCounter = 0x10000;
    }
    mgr->cmdId[slot] = (s32)soundId;
    if (category == 0) {
      s32 masked = (s32)(soundId & 0x7ffffff);
      if (masked < 0x4200000 || masked > 0x5200003) {
        s32 i;
        category = 2;
        for (i = 0; i < 10; i++) {
          if (soundId == g_sndCategory1SoundIds[i]) {
            category = 1;
            break;
          }
        }
      } else {
        category = 4;
      }
    }
    handle = mgr->handleCounter | (category & 7) << 28 | (flag & 1) << 27;
    mgr->cmdHandle[slot] = (s32)handle;
  }
  CoreLockRelease(lock);
  return handle;
}
