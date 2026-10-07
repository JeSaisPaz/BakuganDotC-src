// bdc 0x08899334 BtlAiPadLatchStick
#include "bdc.h"

/* Turns this frame's stick axes of the AI virtual pad into a move request
   (`BtlAiPadCalcStick`), then clears both axes (`stickY`, `stickX`) for the next frame; returns 1
   when a move was requested, else 0. Called by `BtlAiUpdate`. */
s32 BtlAiPadLatchStick(BtlAiPad *self)
{
    s32 moved = BtlAiPadCalcStick(self) != 0;

    self->stickY = 0.0f;
    self->stickX = 0.0f;
    return moved;
}
