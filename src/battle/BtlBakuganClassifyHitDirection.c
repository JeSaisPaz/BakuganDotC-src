// bdc 0x08863f6c BtlBakuganClassifyHitDirection
#include "bdc.h"

/* Classifies where a hit came from relative to the unit's heading (`base.rot[1]`): reverses
   `incomingHeading` (+π, wrapped once into (−π, π]), reduces `heading − reversed` modulo 2π
   (truncating multiple of 2π via `× 1/π`, then +2π when negative) and maps it to a signed
   difference `d` (−x below π, 2π − x otherwise). Returns 1 when `|d|` < π/4 (from the front),
   0 when `|d|` is not ≤ 3π/4 (from behind), else 2 when `d` ≤ 0 and 3 when `d` > 0 (the sides). */
int BtlBakuganClassifyHitDirection(float incomingHeading, BtlBakugan *self)
{
    float reversed = incomingHeading + 3.14159274f;
    float diff;
    float absDiff;

    if (!(reversed <= 3.14159274f)) {
        reversed = reversed - 6.28318548f;
    } else if (reversed <= -3.14159274f) {
        reversed = reversed + 6.28318548f;
    }
    diff = self->base.rot[1] - reversed;
    diff = diff - (float)(s32)(diff * 0.318309873f) * 6.28318548f;
    if (diff < 0.0f) {
        diff = diff + 6.28318548f;
    }
    if (diff < 3.14159274f) {
        diff = -diff;
    } else {
        diff = 6.28318548f - diff;
    }
    absDiff = ABS(diff);
    if (absDiff < 0.785398185f) {
        return 1;
    }
    if (!(absDiff <= 2.35619450f)) {
        return 0;
    }
    if (diff <= 0.0f) {
        return 2;
    }
    return 3;
}
