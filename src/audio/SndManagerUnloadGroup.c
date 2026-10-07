// bdc 0x089c60e0 SndManagerUnloadGroup
#include "bdc.h"

/* Unloads sound group `groupId`: under the manager lock finds the slot whose `groupId` matches and
   returns `SndManagerFreeGroupSlot`'s result for it; returns 1 (nothing to do) when no slot holds
   that group. */

bool SndManagerUnloadGroup(SndManager *mgr, s32 groupId)
{
    bool result = true;
    s32 slot;

    CoreLockAcquire(mgr->lock);
    for (slot = 0; slot < 32; slot++) {
        if (mgr->groups[slot].groupId == groupId) {
            result = SndManagerFreeGroupSlot(mgr, slot);
            break;
        }
    }
    CoreLockRelease(mgr->lock);
    return result;
}
