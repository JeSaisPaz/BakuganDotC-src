// bdc 0x088627f8 BtlBakuganClassifyTargetDirection
#include "bdc.h"

/* Classifies where the unit's target (`BtlBakuganGetTarget`) lies relative to `heading`; returns
   1 when there is no target. With `t = atan2f(dz, dx)` toward the target's `pos`,
   `d = heading - t` is reduced by `trunc(d / π) * 2π`, raised by 2π when negative, and mapped to
   `a = -d` below π, `2π - d` otherwise. Then: |a| < π/4 → 1 (ahead); otherwise not |a| <= 3π/4
   → 0 (behind); otherwise a < 0 → 3, else 2 (the two sides). */
int BtlBakuganClassifyTargetDirection(float heading, BtlBakugan *self)
{
    BtlBakugan *target = BtlBakuganGetTarget(self);
    float targetX;
    float targetZ;
    float diff;
    float angle;

    if (target == NULL) {
        return 1;
    }
    targetX = target->base.pos[0];
    targetZ = target->base.pos[2];
    diff = heading - atan2f(targetZ - self->base.pos[2], targetX - self->base.pos[0]);
    diff = diff - (float)(int)(diff * 0.318309873f) * 6.28318548f;
    if (diff < 0.0f) {
        diff = diff + 6.28318548f;
    }
    if (diff < 3.14159274f) {
        angle = -diff;
    } else {
        angle = 6.28318548f - diff;
    }
    if (ABS(angle) < 0.785398185f) {
        return 1;
    }
    if (!(ABS(angle) <= 2.35619450f)) {
        return 0;
    }
    if (angle < 0.0f) {
        return 3;
    }
    return 2;
}
