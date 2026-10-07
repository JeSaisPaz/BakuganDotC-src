// bdc 0x088fdffc BtlDemoCamPushKeyB
#include "bdc.h"

/* Second key track of the battle demo camera, same shape as `BtlDemoCamPushEyeKey`: the newest
   key of each vector pair moves to the previous slot and `a`/`b` become the new keys (each 4-float
   copy is a VFPU `lv.q`/`sv.q` pair in the original). The frame pair is stored the other way
   round from the eye track: slot 1 moves to slot 0 and slot 1 takes `frame`. */
void BtlDemoCamPushKeyB(BtlDemoCam *self, float *a, float *b, int frame)
{
    int i;

    for (i = 0; i < 4; i++) {
        self->keyBA[1][i] = self->keyBA[0][i];
    }
    for (i = 0; i < 4; i++) {
        self->keyBA[0][i] = a[i];
    }
    for (i = 0; i < 4; i++) {
        self->keyBB[1][i] = self->keyBB[0][i];
    }
    for (i = 0; i < 4; i++) {
        self->keyBB[0][i] = b[i];
    }
    self->keyBFrame[0] = self->keyBFrame[1];
    self->keyBFrame[1] = frame;
}
