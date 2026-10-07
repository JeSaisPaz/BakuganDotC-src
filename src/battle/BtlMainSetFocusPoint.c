// bdc 0x0884c204 BtlMainSetFocusPoint
#include "bdc.h"

/* Stores `pos` (vec4, when non-NULL) as the battle main task's focus target and `frames` as the
   blend counter; `BtlMainUpdateScene` then eases the focus vector toward the target while that
   counter runs. The copy is one VFPU quad load/store (`lv.q`/`sv.q`) of all four floats. */
void BtlMainSetFocusPoint(BtlMain *self, int frames, float *pos)
{
    if (pos != NULL) {
        float x = pos[0];
        float y = pos[1];
        float z = pos[2];
        float w = pos[3];
        self->focusTarget[0] = x;
        self->focusTarget[1] = y;
        self->focusTarget[2] = z;
        self->focusTarget[3] = w;
    }
    self->focusFrames = frames;
}
