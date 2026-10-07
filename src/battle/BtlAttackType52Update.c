// bdc 0x08884440 BtlAttackType52Update
#include "bdc.h"

/* Per-frame handler of attack type 0x52 (handler table `0x08a685f0`, run by `BtlAttackUpdate`): a
   strike from above at the target. On the first frame it moves itself (`pos`) and its effect onto
   the target unit's position (`BtlFindBakuganById` of `targetId`), or, without a target, onto the
   point 500 units ahead of the owner along (cos, 0, sin) of its heading `rot[1]` (w = 0), then
   detaches the effect. From frame 33 to 44 it sweeps a 300-unit vertical segment downward from 300
   above `pos` (`BtlAttackSweepHit`, hit id 0x75, kind 3, mask 0x31bf337e); after frame 49 it ends
   (`BtlAttackEnd`). The binary also copies the source position into a stack slot nothing reads;
   that dead copy is left out. */
void BtlAttackType52Update(BtlAttack *self)
{
    float point[4];
    float to[4];
    float from[4];
    BtlBakugan *target;
    BtlBakugan *owner;
    float angle;

    if (self->age == 0) {
        target = (BtlBakugan *)BtlFindBakuganById(self->targetId);
        if (target != NULL) {
            point[0] = target->base.pos[0];
            point[1] = target->base.pos[1];
            point[2] = target->base.pos[2];
            point[3] = target->base.pos[3];
        } else {
            owner = self->owner;
            angle = owner->base.rot[1];
            /* vrot [C,0,S,0] scaled by 500 (xyz), then + owner pos (xyz); w stays 0 */
            point[0] = __builtin_cosf(angle) * 500.0f + owner->base.pos[0];
            point[1] = 0.0f * 500.0f + owner->base.pos[1];
            point[2] = __builtin_sinf(angle) * 500.0f + owner->base.pos[2];
            point[3] = 0.0f;
        }
        self->pos[0] = point[0];
        self->pos[1] = point[1];
        self->pos[2] = point[2];
        self->pos[3] = point[3];
        ((GfxEffect *)self->effect)->pos[0] = self->pos[0];
        ((GfxEffect *)self->effect)->pos[1] = self->pos[1];
        ((GfxEffect *)self->effect)->pos[2] = self->pos[2];
        ((GfxEffect *)self->effect)->pos[3] = self->pos[3];
        self->effect = NULL;
        return;
    }
    if (self->age >= 0x32) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age > 0x20 && self->age < 0x2d) {
        from[0] = self->pos[0];
        from[1] = self->pos[1];
        from[2] = self->pos[2];
        from[3] = self->pos[3];
        from[1] = from[1] + 300.0f;
        to[0] = 0.0f;
        to[1] = -300.0f;
        to[2] = 0.0f;
        to[3] = 0.0f;
        BtlAttackSweepHit(self->radius, self, from, to, 0x75, 3, 0, 0x31bf337e);
    }
}
