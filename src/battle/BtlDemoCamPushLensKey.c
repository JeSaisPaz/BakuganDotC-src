// bdc 0x088fe03c BtlDemoCamPushLensKey
#include "bdc.h"

/* Pushes a lens key into the battle demo camera: the current lens vector and scalars `a`/`b`
   (field of view / roll style values) move to the previous slot and take the new values, and the
   frame pair shifts and takes `frame`. The original copies the 4-float vector with a VFPU
   `lv.q`/`sv.q` pair. */
void BtlDemoCamPushLensKey(float a, float b, BtlDemoCam *self, float *vec, int frame)
{
    int i;

    for (i = 0; i < 4; i++) {
        self->lensKey[1][i] = self->lensKey[0][i];
    }
    for (i = 0; i < 4; i++) {
        self->lensKey[0][i] = vec[i];
    }
    self->lensA[1] = self->lensA[0];
    self->lensA[0] = a;
    self->lensB[1] = self->lensB[0];
    self->lensB[0] = b;
    self->lensFrame[1] = self->lensFrame[0];
    self->lensFrame[0] = frame;
}
