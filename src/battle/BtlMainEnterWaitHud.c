// bdc 0x0884dfa4 BtlMainEnterWaitHud
#include "bdc.h"

/* Saves the current phase and switches the battle main task to phase 8
   (`BtlMainPhaseWaitHud`); called by `UiTalkRequestClose`. */
void BtlMainEnterWaitHud(BtlMain *self)
{
    self->savedPhase = self->phase;
    self->phase = 8;
    self->drawPhase = 8;
    self->field10 = 0;
    self->phaseStep = 0;
}
