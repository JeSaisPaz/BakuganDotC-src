// bdc 0x08862cd8 BtlBakuganSetMotionSpeed
#include "bdc.h"

/* Motion-speed virtual of the battle units (vtable `0x08af1fa4` slot `+0x30`, overriding
   `GfxModelSetMotionSpeed`): unless flag `+0x144 & 0x100` is set, multiplies `speed` by the
   status speed factor (`BtlBakuganGetSpeedStatusFactor`), then sets it with
   `GfxModelSetMotionSpeed`. Returns the previous speed `+0xb4`. */
float BtlBakuganSetMotionSpeed(float speed, BtlBakugan *self)
{
    float previous = self->base.motionSpeed;

    if ((self->stateFlags & 0x100) == 0) {
        speed = speed * BtlBakuganGetSpeedStatusFactor(self);
    }
    GfxModelSetMotionSpeed(&self->base, speed);
    return previous;
}
