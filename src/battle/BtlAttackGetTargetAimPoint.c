// bdc 0x08879fa4 BtlAttackGetTargetAimPoint
#include "bdc.h"

/* Writes the aim point of an attack into `out` (vec4). With a target unit (`targetId`,
   `BtlFindBakuganById`): the target's position (all four lanes) with y raised by 500. Without
   one: a point 1000 units along the owner's heading `rot[1]` (x = cos * 1000, z = sin * 1000),
   with y = 500, offset by the owner's position (x, y, z); w is 0. Used by
   `BtlAttackType7CUpdate`. */
void BtlAttackGetTargetAimPoint(BtlAttack *self, float *out)
{
    BtlBakugan *target = (BtlBakugan *)BtlFindBakuganById(self->targetId);
    float yaw;
    float *pos;

    if (target != NULL) {
        out[0] = target->base.pos[0];
        out[1] = target->base.pos[1];
        out[2] = target->base.pos[2];
        out[3] = target->base.pos[3];
        out[1] = out[1] + 500.0f;
        return;
    }
    /* vmul.s by the bank's 2/pi then vrot [C,0,S,0]: cos/sin of the angle in radians */
    yaw = self->owner->base.rot[1];
    out[0] = __builtin_cosf(yaw) * 1000.0f;
    out[1] = 0.0f * 1000.0f;
    out[2] = __builtin_sinf(yaw) * 1000.0f;
    out[3] = 0.0f;
    out[1] = 500.0f;
    pos = self->owner->base.pos;
    out[0] = out[0] + pos[0];
    out[1] = out[1] + pos[1];
    out[2] = out[2] + pos[2];
}
