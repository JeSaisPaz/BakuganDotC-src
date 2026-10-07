// bdc 0x088fdfbc BtlDemoCamPushEyeKey
#include "bdc.h"

/* Pushes a new camera key into the battle demo camera (`BtlDemoCamCtor`): the current eye key
   moves to the previous slot and `eye` becomes current, the look-at key likewise with `lookAt`,
   and the current key frame moves to the previous slot with `frame` as the new one. The original
   copies each 4-float key with a VFPU `lv.q`/`sv.q` pair. */
void BtlDemoCamPushEyeKey(BtlDemoCam *self, float *eye, float *lookAt, int frame)
{
    int i;

    for (i = 0; i < 4; i++) {
        self->eyeKey[1][i] = self->eyeKey[0][i];
    }
    for (i = 0; i < 4; i++) {
        self->eyeKey[0][i] = eye[i];
    }
    for (i = 0; i < 4; i++) {
        self->lookKey[1][i] = self->lookKey[0][i];
    }
    for (i = 0; i < 4; i++) {
        self->lookKey[0][i] = lookAt[i];
    }
    self->eyeFrame[1] = self->eyeFrame[0];
    self->eyeFrame[0] = frame;
}
