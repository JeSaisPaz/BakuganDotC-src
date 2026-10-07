// bdc 0x0886235c BtlBakuganApplyVelocity
#include "bdc.h"

/* Moves the Bakugan by `velocity` for this frame. Sets stateFlags bit 1, clamps the vertical speed
   to >= -180 and, while it is not <= 0 and the unit is less than 100 below the arena ceiling
   (BtlStageGetCeilingHeight), scales it by (ceiling - y) / 100. Marks the unit as moving (bit 30;
   `wasMoving` = bit 30 before), then either moves once with collision (BtlBakuganMoveCollide)
   and pushes apart from other units (BtlBakuganPushApartFromUnits), or, when |velocity| exceeds
   0.8 x radius, does both in floor(|v| / (0.8 x radius)) + 1 equal sub-steps. Sets bit 31 when
   the unit was moving and bit 30 got cleared meanwhile. Returns the ground result: the single
   step's value, or 1 when any sub-step reported ground. */

u32 BtlBakuganApplyVelocity(BtlBakugan *self, float *velocity)
{
    float step[4] __attribute__((aligned(16)));
    float moveDelta[4] __attribute__((aligned(16)));
    float vy;
    float belowCeiling;
    float len;
    float stepCount;
    float invCount;
    s32 count;
    s32 i;
    u32 wasMoving;
    u32 onGround;

    self->stateFlags |= 2;
    vy = velocity[1];
    if (vy <= -180.0f) {
        vy = -180.0f;
    }
    velocity[1] = vy;
    onGround = 0;
    if (!(vy <= 0.0f)) {
        belowCeiling = self->base.pos[1] - BtlStageGetCeilingHeight();
        if (!(belowCeiling <= -100.0f)) {
            velocity[1] = velocity[1] * (belowCeiling * -0.00999999978f);
        }
    }

    wasMoving = (self->stateFlags & 0x40000000) != 0;
    self->stateFlags |= 0x40000000;

    len = __builtin_sqrtf(velocity[0] * velocity[0] + velocity[1] * velocity[1] +
                          velocity[2] * velocity[2]);

    if (self->radius * 0.800000012f < len) {
        stepCount = (float)(s32)floorf(len / (self->radius * 0.800000012f)) + 1.0f;
        count = (s32)stepCount;
        invCount = 1.0f / stepCount;
        /* step.xyz = velocity.xyz / count; step.w comes from the bank constant S713 (0.0f). */
        step[0] = velocity[0] * invCount;
        step[1] = velocity[1] * invCount;
        step[2] = velocity[2] * invCount;
        step[3] = 0.0f;
        for (i = 0; i < count; i++) {
            moveDelta[0] = step[0];
            moveDelta[1] = step[1];
            moveDelta[2] = step[2];
            moveDelta[3] = step[3];
            onGround = (onGround | (u32)BtlBakuganMoveCollide(self, moveDelta, wasMoving)) != 0;
            BtlBakuganPushApartFromUnits(self, step, wasMoving);
        }
    } else {
        moveDelta[0] = velocity[0];
        moveDelta[1] = velocity[1];
        moveDelta[2] = velocity[2];
        moveDelta[3] = velocity[3];
        onGround = (u32)BtlBakuganMoveCollide(self, moveDelta, wasMoving);
        BtlBakuganPushApartFromUnits(self, velocity, wasMoving);
    }

    if (wasMoving && (self->stateFlags & 0x40000000) == 0) {
        self->stateFlags |= 0x80000000;
    }
    return onGround;
}
