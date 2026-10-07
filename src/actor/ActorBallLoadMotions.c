// bdc 0x088b7e18 ActorBallLoadMotions
#include "bdc.h"

/* Loads the motions of an `ActorBall` (`ActorBallCtor`): enables motion playback on the model
   (`GfxModelEnableMotion`); when the first motion of the kind's `{slot, name}` list
   (`g_actorBallMotionLists``[kind-0x58]`) is not registered yet, loads the kind's motion pack
   (`g_actorBallMotionFiles``[kind-0x58]`, e.g. `"00sp_dor_mot.gmo"`) into `motionFile`; then
   allocates the 5-entry slot→motion index map `motionMap` from the low heap, fills it with
   `0xffff` and stores the index of each listed motion (`GmoMotionIndexOfName`) at its slot.
   Does nothing without a motion manager. */

void ActorBallLoadMotions(CoreObject *ball)
{
    ActorBall *self = (ActorBall *)ball;
    const ActorBallMotionEntry *entry;
    s32 kindIdx;
    s32 index;
    bool fromLow;
    u16 *map;
    s32 i;

    if (!GmoMotionMgrExists()) {
        return;
    }
    GfxModelEnableMotion(&self->base);
    kindIdx = (s32)self->base.base.unk08 - 0x58;
    entry = g_actorBallMotionLists[kindIdx];
    index = GmoMotionIndexOfName(GmoMotionMgrGet(), entry->name);
    if (index == -1) {
        self->motionFile = GmoMotionLoadFile(GmoMotionMgrGet(), g_actorBallMotionFiles[kindIdx]);
    }

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    map = (u16 *)MemAlloc(10, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->motionMap = map;

    for (i = 0; i < 5; i++) {
        self->motionMap[i] = 0xffff;
    }
    for (i = 0; i < 5; i++) {
        if (entry->slot >= 5) {
            return;
        }
        index = GmoMotionIndexOfName(GmoMotionMgrGet(), entry->name);
        self->motionMap[entry->slot] = (u16)index;
        entry++;
    }
}
