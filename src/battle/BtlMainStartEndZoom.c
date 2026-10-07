// bdc 0x0884b3d4 BtlMainStartEndZoom
#include "bdc.h"

/* Starts the battle-end zoom of `BtlMainUpdateBattleEnd`: frame count `frames` (0 is replaced
   by 1; negative counts pass through), target `amount`, parameter `param`, progress 0 and per-frame step `amount / frames`. */
void BtlMainStartEndZoom(BtlMain *self, int frames, float amount, float param)
{
    if (frames == 0) {
        frames = 1;
    }
    self->endZoomFrames = frames;
    self->endZoomTarget = amount;
    self->endZoomParam = param;
    self->endZoomProgress = 0.0f;
    self->endZoomStep = amount / (float)frames;
}
