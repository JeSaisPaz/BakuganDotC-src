// bdc 0x0898f710 UiCollectionFigureAutoRotateModel
#include "bdc.h"

/* Idle auto-rotation in the second detail view of `UiCollectionFigure`:
   counts the idle timer up to 180 frames, then turns the model's yaw by 0.015 rad per frame. */

void UiCollectionFigureAutoRotateModel(UiCollectionFigure *self)
{
    if (self->idleTimer < 180.0f) {
        self->idleTimer = self->idleTimer + 1.0f;
        return;
    }
    self->yaw = self->yaw + 0.015f;
}
