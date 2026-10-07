// bdc 0x0882d0b8 BtlHudPhaseWaitStart
#include "bdc.h"

/* HUD phase 0 handler (`BtlHudUpdate`): while `phaseStep` is 0 it waits for UI window kind 1 to
   be active (`UiGetWindowActive`); once it is (or when `phaseStep` was already non-zero) it
   resets `phaseStep` to 0 and advances `phase` by one, to `BtlHudPhaseBuild`. */
void BtlHudPhaseWaitStart(BtlHud *self)
{
    s32 step = self->phaseStep;

    if (step == 0) {
        if (!UiGetWindowActive(1)) {
            return;
        }
        self->phaseStep = step + 1; /* overwritten just below; the binary stores both */
    }
    self->phaseStep = 0;
    self->phase = self->phase + 1;
}
