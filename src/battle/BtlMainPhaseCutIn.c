// bdc 0x0884fb5c BtlMainPhaseCutIn
#include "bdc.h"

/* Phase 5 of the battle main task (entry 5 of `g_btlMainUpdateTable`): runs the battle phase
   (`BtlMainPhaseBattle`) every frame and, once no task with id 0x1e1 exists (`CoreTaskExists`),
   sets `phase`/`drawPhase` to 1 and resumes the event script (`BtlStageResumeEventScript`) while
   `g_btlBattleOutcome` is 0, or sets both to 2 when an outcome is set. */

void BtlMainPhaseCutIn(BtlMain *self)
{
    BtlMainPhaseBattle(self);
    if (CoreTaskExists(0x1e1) == 0) {
        if (g_btlBattleOutcome == 0) {
            self->phase = 1;
            self->drawPhase = 1;
            BtlStageResumeEventScript();
        } else {
            self->phase = 2;
            self->drawPhase = 2;
        }
    }
}
