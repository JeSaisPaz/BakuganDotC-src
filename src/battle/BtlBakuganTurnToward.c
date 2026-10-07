// bdc 0x088625c8 BtlBakuganTurnToward
#include "bdc.h"

/* Turns the unit's heading (`rot[1]`) toward `angle`: reduces `heading - angle` by whole turns
   (truncating `diff / pi` and subtracting that many 2*pi, then adding 2*pi when negative), turns
   it into the signed step toward `angle` (`-diff` below pi, else `2*pi - diff`), scales it by
   `rate`, clamps it to +-`maxStep` when `maxStep` is non-zero, adds it to the heading and wraps
   the heading once into (-pi, pi]. Returns the unscaled signed step (left in f0). */
float BtlBakuganTurnToward(float angle, float rate, float maxStep, BtlBakugan *self)
{
    float diff;
    float step;
    float heading;

    diff = self->base.rot[1] - angle;
    diff = diff - (float)(s32)(diff * 0.318309873f) * 6.28318548f;
    if (diff < 0.0f) {
        diff = diff + 6.28318548f;
    }
    if (diff < 3.14159274f) {
        step = -diff;
    } else {
        step = 6.28318548f - diff;
    }
    diff = step * rate;
    if (maxStep != 0.0f) {
        if (!(diff <= maxStep)) {
            diff = maxStep;
        } else if (diff < -maxStep) {
            diff = -maxStep;
        }
    }
    self->base.rot[1] = self->base.rot[1] + diff;
    heading = self->base.rot[1];
    if (!(heading <= 3.14159274f)) {
        self->base.rot[1] = heading - 6.28318548f;
    } else if (heading <= -3.14159274f) {
        self->base.rot[1] = heading + 6.28318548f;
    }
    return step;
}
