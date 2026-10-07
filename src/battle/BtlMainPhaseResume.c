// bdc 0x0884fcf4 BtlMainPhaseResume
#include "bdc.h"

/* Phase 7 of the battle main task: returns to phase 1 (battle) and clears the phase step. */
void BtlMainPhaseResume(BtlMain *self)
{
    self->phase = 1;
    self->phaseStep = 0;
}
