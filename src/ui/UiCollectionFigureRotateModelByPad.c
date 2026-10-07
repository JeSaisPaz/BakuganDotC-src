// bdc 0x0898f75c UiCollectionFigureRotateModelByPad
#include "bdc.h"

/* Turns the model in the second detail view of `UiCollectionFigure` with
   the held D-pad (`pad->buttons`): up/down change `pitch` by +/-0.03 (clamped to +/-0.78),
   left/right change `yaw` by -/+0.03 (unclamped); any change resets `idleTimer`, used by
   `UiCollectionFigureAutoRotateModel`. */

void UiCollectionFigureRotateModelByPad(UiCollectionFigure *self)
{
    PadState *pad = self->base.pad;
    float oldPitch = self->pitch;
    float pitch = oldPitch;
    float oldYaw = self->yaw;

    if (pad->buttons & 0x10) {
        pitch = oldPitch + 0.03f;
        self->pitch = pitch;
        if (!(pitch <= 0.78f)) {
            self->pitch = 0.78f;
            pitch = 0.78f;
        }
    } else if (pad->buttons & 0x40) {
        pitch = oldPitch - 0.03f;
        self->pitch = pitch;
        if (pitch < -0.78f) {
            self->pitch = -0.78f;
            pitch = -0.78f;
        }
    }

    if (pad->buttons & 0x80) {
        self->yaw = oldYaw - 0.03f;
    } else if (pad->buttons & 0x20) {
        self->yaw = oldYaw + 0.03f;
    }

    if (!(oldPitch == pitch) || !(oldYaw == self->yaw)) {
        self->idleTimer = 0.0f;
    }
}
