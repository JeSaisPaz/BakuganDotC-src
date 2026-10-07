// bdc 0x088fe07c BtlDemoCamResetKeys
#include "bdc.h"

/* Resets the key tracks of the battle demo camera (`BtlDemoCam`, `BtlDemoCamCtor`): zeroes both
   slots of the lens, eye and look-at vector keys (sv.q of the bank zero vector C720), `auxKey`,
   the current frame and the eye/lens key frames, sets the lens scalars a to 1.0 and b to 0.0, and
   sets the field of view to 45.0. */
void BtlDemoCamResetKeys(BtlDemoCam *self)
{
    int i;

    self->auxKey[2] = 0.0f;
    self->auxKey[1] = 0.0f;
    for (i = 0; i < 4; i++) {
        self->lensKey[0][i] = 0.0f;
    }
    for (i = 0; i < 4; i++) {
        self->lensKey[1][i] = 0.0f;
    }
    for (i = 0; i < 4; i++) {
        self->eyeKey[0][i] = 0.0f;
    }
    for (i = 0; i < 4; i++) {
        self->eyeKey[1][i] = 0.0f;
    }
    for (i = 0; i < 4; i++) {
        self->lookKey[0][i] = 0.0f;
    }
    for (i = 0; i < 4; i++) {
        self->lookKey[1][i] = 0.0f;
    }
    self->auxKey[0] = 0.0f;
    self->eyeFrame[0] = 0;
    self->eyeFrame[1] = 0;
    self->lensFrame[0] = 0;
    self->lensFrame[1] = 0;
    self->frame = 0;
    self->lensA[0] = 1.0f;
    self->lensA[1] = 1.0f;
    self->lensB[0] = 0.0f;
    self->lensB[1] = 0.0f;
    self->fov = 45.0f;
}
