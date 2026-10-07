// bdc 0x08865dd0 BtlCalcDeceleratingDistance
#include "bdc.h"

/* Returns the distance covered in `frames` frames when starting at `speed` and losing `decel` per
   frame: the sum over i < frames of (speed - i*decel), accumulated frame by frame. Used by
   `BtlBakuganState04Update` and `BtlBakuganState07Update` to predict landing/lunge distance. */
float BtlCalcDeceleratingDistance(float speed, float decel, u32 unused, int frames)
{
    float dist = 0.0f;
    int i;

    (void)unused;
    for (i = 0; i < frames; i++) {
        dist = speed + dist;
        speed = speed - decel;
    }
    return dist;
}
