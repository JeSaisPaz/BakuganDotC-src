// bdc 0x088847b4 BtlInputReset
#include "bdc.h"

/* Resets the input controller object read by `BtlInputReadActions`: clears the heading, stick
   and cooldown words and the disabled flag, zeroes the two 4-float vectors `dir`/`stick` (stored
   from the VFPU bank constant C720 = (0,0,0,0)), sets the repeat parameter to 0x40 and the
   allowed-action mask to -1 (all actions). */
void BtlInputReset(BtlInput *self)
{
    int i;

    self->heading = 0.0f;
    self->moveScale = 0.0f;
    self->stickMagnitude = 0.0f;
    self->headingDot = 0.0f;
    for (i = 0; i < 4; i++) {
        self->dir[i] = 0.0f;
    }
    for (i = 0; i < 4; i++) {
        self->stick[i] = 0.0f;
    }
    self->holdFrames = 0;
    self->aiActions = 0;
    self->chargeCooldown = 0;
    self->dodgeCooldown = 0;
    self->attackCooldown = 0;
    self->stickHeading = 0.0f;
    self->disabled = 0;
    self->queuedActions = 0x40;
    self->allowedActions = 0xffffffff;
}
