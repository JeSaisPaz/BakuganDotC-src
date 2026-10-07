// bdc 0x089dfbd4 GfxModelUpdateMotion
#include "bdc.h"

/* Per-frame motion step of a model. Returns 0 when motion is not enabled (`motionEnabled`).
   Otherwise temporarily sets the end frame of the current motion slot (range word 1 of the record's
   data area) to the larger of end/start, advances the motion (`GfxModelAdvanceMotion`), sets
   `motionEnded` when the frame did not move forward (positive `motionSpeed`) or did not move
   backward (negative speed), restores the original end frame (`GfxModelSetMotionEnd` and a
   direct store) and returns the new frame (`GfxModelMotionFrame`). */

float GfxModelUpdateMotion(GfxModel *self)
{
    float end;
    float start;
    float savedEnd;
    float before;
    float after;
    float *range;

    if (self->motionEnabled == 0) {
        return 0.0f;
    }
    end = GfxModelGetMotionEnd(self);
    start = GfxModelGetMotionStart(self);
    if (end < start) {
        end = start;
    }
    range = (float *)((GmoMotionRecord *)self->data->motions)[self->motionSlot].data;
    savedEnd = range[1];
    range[1] = end;
    before = GfxModelMotionFrame(self);
    GfxModelAdvanceMotion(self);
    after = GfxModelMotionFrame(self);
    if (self->motionSpeed < 0.0f) {
        if (!(after < before)) {
            self->motionEnded = 1;
        }
    } else if (!(self->motionSpeed <= 0.0f) && after <= before) {
        self->motionEnded = 1;
    }
    GfxModelSetMotionEnd(self, savedEnd);
    range = (float *)((GmoMotionRecord *)self->data->motions)[self->motionSlot].data;
    range[1] = savedEnd;
    return after;
}
