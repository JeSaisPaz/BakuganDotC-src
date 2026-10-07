// bdc 0x088b8828 ActorBallFreeMotions
#include "bdc.h"

/* Frees the motions an `ActorBall` kind loaded (`ActorBallLoadMotions`): walks the `{slot, name}`
   list `0x08abd748[kind-0x58]` (kind index clamped to 0..19) and frees every resident motion by
   name (`GmoMotionFreeByName`). */

void ActorBallFreeMotions(CoreObject *ball)
{
    const ActorBallMotionEntry *entry;
    s32 kind;
    s32 i;

    if (!GmoMotionMgrExists()) {
        return;
    }
    kind = (s32)ball->unk08 - 0x58;
    if (kind < 0) {
        kind = 0;
    } else if (kind > 0x13) {
        kind = 0x13;
    }
    entry = g_actorBallMotionLists[kind];
    for (i = 0; i < 5; i++, entry++) {
        if (entry->slot > 4) {
            return;
        }
        if (GmoMotionIndexOfName(GmoMotionMgrGet(), entry->name) != -1) {
            GmoMotionFreeByName(GmoMotionMgrGet(), entry->name);
        }
    }
}
