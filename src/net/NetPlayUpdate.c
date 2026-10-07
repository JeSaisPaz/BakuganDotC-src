// bdc 0x0881bba4 NetPlayUpdate
#include "bdc.h"

/* Per-frame state-machine step of the NetPlay manager, called by `BootMainThread` right before
   the task update whenever `NetPlayHasManager` is true. Clears `updateRan` (`+0xde`) and returns
   at once while a system utility dialog is active (`SysUtilIsInit` / `SysUtilIsBusy`).
   Otherwise, for a state `+4` in 0..8, it first forces state 8 (substate 0) when an abort was
   requested (`abortRequested`, `NetPlayRequestAbort`), then calls the `MemberFnPtr` handler
   `g_netPlayStateFns[state]` (`g_netPlayStateFns`) on the object. Afterwards, in state 7 (in
   session) with `NetModeFlagIsClear` false and `NetSyncStateIs2` true, it clears `synced`
   (`+0xb9`) and `newFrame` (`+0xb8`) and, when the local net character exists
   (`NetCharaGetByIndex`(0)), both records of its front frame carry the `NetPlayGetFlags` bits
   (`NetCharaBothHaveFlags`) and `NetCharaHasNewSyncedFrame` reports a frame, sets `synced`
   and, if that frame is above `maxFrameSeen` (`+0xe8`), records it and sets `newFrame`. In state
   7 with `NetSyncStateIs2` false both bytes are left unchanged; in state 7 with
   `NetModeFlagIsClear` true, and in every other state, both are set to 1. Finally sets
   `updateRan` and calls `NetPlayShutdown` once `finished` (`+0xd`) is set. */

void NetPlayUpdate(NetPlay *self)
{
    NetChara *chara;
    s32 frame;
    s32 state;

    self->updateRan = 0;
    if (SysUtilIsInit() && SysUtilIsBusy(SysUtilGetCell())) {
        return;
    }

    state = self->state;
    if (state >= 0 && state < 9) {
        const MemberFnPtr *member;
        u8 *obj;
        void *fn;

        if (state != 8 && self->abortRequested) {
            self->state = 8;
            self->substate = 0;
            state = 8;
        }
        member = &g_netPlayStateFns[state];
        obj = (u8 *)self + member->delta;
        fn = member->pfn;
        /* GCC 2.x pointer-to-member: index != 0 is a virtual slot, pfn then holds the vptr offset */
        if (member->index != 0) {
            const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)member->pfn);
            const VtblEntry *entry = &vtbl[member->index];

            fn = entry->fn;
            obj += entry->delta;
        }
        ((void (*)(void *))fn)(obj);
        state = self->state;
    }

    if (state == 7) {
        if (NetModeFlagIsClear()) {
            self->synced = 1;
            self->newFrame = 1;
        } else if (NetSyncStateIs2()) {
            chara = NetCharaGetByIndex(0);
            self->synced = 0;
            self->newFrame = 0;
            if (chara != NULL) {
                u32 flags = NetPlayGetFlags(self);

                frame = -1;
                if (NetCharaBothHaveFlags(chara, flags) &&
                    NetCharaHasNewSyncedFrame(chara, &frame)) {
                    self->synced = 1;
                    if (self->maxFrameSeen < frame) {
                        self->maxFrameSeen = frame;
                        self->newFrame = 1;
                    }
                }
            }
        }
    } else {
        self->synced = 1;
        self->newFrame = 1;
    }

    self->updateRan = 1;
    if (self->finished) {
        NetPlayShutdown();
    }
}
