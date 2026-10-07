// bdc 0x089c14dc SndGroupLoaderRequest
#include "bdc.h"

/* Registers that sound `soundId` (voice handle `handle`) is in use so that its sound group stays
   loaded. Only acts when the loader is `enabled` and the group index `soundId >> 20 & 0xff`
   (invalid indices above 0x52 become -1) is 0 or 1: under the loader lock, if a request record for
   `soundId` already exists it just sets its `touched` byte; otherwise, when the group is not yet in
   any `SndManager` slot (`SndManagerFindGroupSlot(..., 1) == -1`) and `SndManagerLoadGroup`
   accepted it, it adds a 12-byte `SndGroupRequest` `{soundId, handle, released = 0, touched = 1}`
   (priority 1000, from the request pool or the low heap) and, if the group has no hold record yet,
   an 8-byte `SndGroupHold` `{groupId, ttl = 30}`. */

void SndGroupLoaderRequest(SndGroupLoader *loader, s32 soundId, s32 handle)
{
    CoreListNode *node;
    SndGroupRequest *req;
    SndGroupRequest *pooledReq;
    SndGroupHold *hold;
    SndGroupHold *pooledHold;
    bool found;
    bool low;
    s32 groupId;

    if (loader->enabled == 0) {
        return;
    }
    groupId = soundId >> 20 & 0xff;
    found = false;
    if (groupId >= 0x53) {
        groupId = -1;
    }
    if (groupId < 0 || groupId >= 2) {
        return;
    }

    CoreLockAcquire(loader->lock);
    for (node = SndRequestListFirst(loader->requests); node != NULL; node = node->next) {
        req = (SndGroupRequest *)node->data;
        if (req != NULL && req->soundId == soundId) {
            req->touched = 1;
            found = true;
            break;
        }
    }
    CoreLockRelease(loader->lock);

    if (found) {
        return;
    }
    if (!SndHasManager()) {
        return;
    }
    if (SndManagerFindGroupSlot(SndGetManager(), groupId, true) != -1) {
        return;
    }
    if (!SndManagerLoadGroup(SndGetManager(), groupId)) {
        return;
    }

    CoreLockAcquire(loader->lock);
    req = NULL;
    if (loader->requestPool != NULL) {
        pooledReq = (SndGroupRequest *)MemPoolAlloc(loader->requestPool);
        if (pooledReq != NULL) {
            req = pooledReq;
        }
    }
    if (req == NULL) {
        MemLock();
        low = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        req = (SndGroupRequest *)MemAlloc(sizeof(SndGroupRequest), NULL, 0);
        MemSetAllocFromLow(low);
        MemUnlock();
    }
    req->soundId = soundId;
    req->handle = handle;
    req->released = 0;
    req->touched = 1;
    SndRequestListInsert(loader->requests, req, 1000);

    if (groupId != -1) {
        found = false;
        for (node = SndGroupHoldListFirst(loader->holds); node != NULL; node = node->next) {
            hold = (SndGroupHold *)node->data;
            if (hold != NULL && hold->groupId == groupId) {
                found = true;
                break;
            }
        }
        if (!found) {
            hold = NULL;
            if (loader->holdPool != NULL) {
                pooledHold = (SndGroupHold *)MemPoolAlloc(loader->holdPool);
                if (pooledHold != NULL) {
                    hold = pooledHold;
                }
            }
            if (hold == NULL) {
                MemLock();
                low = MemIsAllocFromLow();
                MemSetAllocFromLow(true);
                hold = (SndGroupHold *)MemAlloc(sizeof(SndGroupHold), NULL, 0);
                MemSetAllocFromLow(low);
                MemUnlock();
            }
            hold->groupId = groupId;
            hold->ttl = 30;
            SndGroupHoldListInsert(loader->holds, hold, 1000);
        }
    }
    CoreLockRelease(loader->lock);
}
