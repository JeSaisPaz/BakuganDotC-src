// bdc 0x0884b768 BtlMainClearEndZoom
#include "bdc.h"

/* Clears the battle-end zoom fields set by `BtlMainStartEndZoom`. */
void BtlMainClearEndZoom(BtlMain *self)
{
    self->endZoomFrames = 0;
    self->endZoomTarget = 0.0f;
    self->endZoomParam = 0.0f;
    self->endZoomProgress = 0.0f;
    self->endZoomStep = 0.0f;
}
