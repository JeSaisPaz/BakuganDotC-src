// bdc 0x0884fd04 BtlMainPhaseWaitHud
#include "bdc.h"

/* Phase 8 of the battle main task (entered by `BtlMainEnterWaitHud`), driven by `phaseStep`
   with fall-through steps: step 0 locks all units (`BtlSetControlLockAll``(1)`) and, when the
   talk task exists, requests its close (`UiTalkSetCloseRequest`); steps 0..1 clear
   `flashTarget` and jump to step 10; step 10 advances to 11; step 11 waits until UI window 0xb is
   active. Then (and immediately for any other step value) it sets byte `field10`, restores the
   saved phase `savedPhase` into `phase` and `drawPhase`, resets `phaseStep` and unlocks the units
   unless the restored phase is 6. Always ends with `BtlMainUpdateScene`. */

void BtlMainPhaseWaitHud(BtlMain *self)
{
    int phase;

    switch (self->phaseStep) {
    case 0:
        BtlSetControlLockAll(1);
        if (UiTalkTaskExists()) {
            UiTalkSetCloseRequest(UiGetTalkTask());
        }
        self->flashTarget = 0.0f;
        self->phaseStep++;
        /* fall through */
    case 1:
        self->phaseStep = 10;
        self->flashTarget = 0.0f;
        /* fall through */
    case 10:
        self->phaseStep++;
        /* fall through */
    case 11:
        if (!UiGetWindowActive(0xb)) {
            BtlMainUpdateScene(self);
            return;
        }
        self->phaseStep++;
        break;
    default:
        break;
    }
    phase = self->savedPhase;
    self->field10 = 1;
    self->phase = phase;
    self->drawPhase = phase;
    self->phaseStep = 0;
    if (phase != 6) {
        BtlSetControlLockAll(0);
    }
    BtlMainUpdateScene(self);
}
