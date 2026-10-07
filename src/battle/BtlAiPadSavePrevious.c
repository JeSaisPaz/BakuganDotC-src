// bdc 0x0889937c BtlAiPadSavePrevious
#include "bdc.h"

/* Saves the AI virtual pad's current input controller as the previous-frame one: copies the
   fields of `cur` from `owner` through `mode` (bytes +0x00..+0x5f, not the whole 0x70-byte
   controller) into `prev`. Called by `BtlAiUpdate`. */
void BtlAiPadSavePrevious(BtlAiPad *self)
{
    int i;

    self->prev.owner = self->cur.owner;
    self->prev.heading = self->cur.heading;
    self->prev.moveScale = self->cur.moveScale;
    self->prev.disabled = self->cur.disabled;
    self->prev.queuedActions = self->cur.queuedActions;
    self->prev.allowedActions = self->cur.allowedActions;
    self->prev.holdFrames = self->cur.holdFrames;
    self->prev.aiActions = self->cur.aiActions;
    self->prev.stickMagnitude = self->cur.stickMagnitude;
    self->prev.headingDot = self->cur.headingDot;
    self->prev.stickHeading = self->cur.stickHeading;
    for (i = 0; i < 4; i++) {
        self->prev.dir[i] = self->cur.dir[i];
    }
    for (i = 0; i < 4; i++) {
        self->prev.stick[i] = self->cur.stick[i];
    }
    self->prev.chargeCooldown = self->cur.chargeCooldown;
    self->prev.dodgeCooldown = self->cur.dodgeCooldown;
    self->prev.attackCooldown = self->cur.attackCooldown;
    self->prev.mode = self->cur.mode;
}
