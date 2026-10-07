// bdc 0x089c1920 SndGroupLoaderUpdate
#include "bdc.h"

/* Per-frame update of the `SndGroupLoader`, run at the end of `SndEmitterUpdateAll`. Under the
   loader lock it rebuilds the needed-group list from the live requests and then loads/unloads
   accordingly: each request whose group index is valid adds that group to the list (only while the
   loader is `enabled`); a request whose `touched` byte is set just has it cleared, while an
   untouched but `released` one is deleted once its group has no live voice
   (`SndManagerIsGroupPlaying`). Each hold record then either has its `ttl` reset to 30 while its
   group is still needed, or, if the group is no longer needed (or the loader is disabled), is
   dropped after `SndManagerUnloadGroup` succeeds (the group is not loading and not playing); with
   no sound manager the hold is simply dropped. */

void SndGroupLoaderUpdate(SndGroupLoader *loader)
{
    CoreListNode *node;
    CoreListNode *next;
    SndGroupRequest *req;
    SndGroupHold *hold;
    s32 group;

    CoreLockAcquire(loader->lock);
    SndGroupIdListClear(loader->neededGroups);
    SndRequestListPurgeRemoved(loader->requests);

    for (node = SndRequestListFirst(loader->requests); node != NULL; node = next) {
        req = (SndGroupRequest *)node->data;
        next = node->next;
        if (req == NULL) {
            continue;
        }
        group = req->soundId >> 20 & 0xff;
        if (group < 0x53 && SndGroupIdListIndexOf(loader->neededGroups, group) == -1 &&
            loader->enabled != 0) {
            SndGroupIdListInsert(loader->neededGroups, group, 1000);
        }
        if (req->touched != 0) {
            req->touched = 0;
        } else if (req->released != 0 &&
                   !SndManagerIsGroupPlaying(SndGetManager(), (u32)group) &&
                   SndRequestListRemove(loader->requests, req)) {
            if (loader->requestPool != NULL && MemPoolFree(loader->requestPool, req)) {
                req = NULL;
            }
            if (req != NULL) {
                MemLock();
                MemFree(req, NULL, 0);
                MemUnlock();
            }
        }
    }

    SndGroupIdListPurgeRemoved(loader->neededGroups);
    SndGroupHoldListPurgeRemoved(loader->holds);

    for (node = SndGroupHoldListFirst(loader->holds); node != NULL; node = next) {
        hold = (SndGroupHold *)node->data;
        next = node->next;
        if (hold == NULL) {
            continue;
        }
        group = hold->groupId;
        if (SndGroupIdListIndexOf(loader->neededGroups, group) != -1 && loader->enabled != 0) {
            hold->ttl = 30;
            continue;
        }
        if (SndHasManager()) {
            if (SndManagerQueryGroupLoad(SndGetManager(), group)) {
                continue;
            }
            if (SndManagerIsGroupPlaying(SndGetManager(), (u32)group)) {
                continue;
            }
            if (!SndManagerUnloadGroup(SndGetManager(), group)) {
                continue;
            }
        }
        if (SndGroupHoldListRemove(loader->holds, hold)) {
            if (loader->holdPool != NULL && MemPoolFree(loader->holdPool, hold)) {
                hold = NULL;
            }
            if (hold != NULL) {
                MemLock();
                MemFree(hold, NULL, 0);
                MemUnlock();
            }
        }
    }

    CoreLockRelease(loader->lock);
}
