// bdc 0x0884ddd4 BtlMainStartTalk
#include "bdc.h"

/* Unless task 0x1e0 runs, stores `unit` in `talkUnit` and switches the battle main task to phase 4
   (`BtlMainPhaseTalk`, sub-state 1), then suspends the stage event script
   (`BtlStageSuspendEventScript`). */
void BtlMainStartTalk(BtlMain *self, BtlBakugan *unit)
{
    if (CoreTaskExists(0x1e0) == 0) {
        self->talkUnit = unit;
        self->phase = 4;
        self->drawPhase = 4;
        self->talkStep = 1;
        BtlStageSuspendEventScript();
    }
}
