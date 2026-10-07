// bdc 0x0887d75c BtlAttackType1CUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x1c (handler table `0x08a685f0`, run by `BtlAttackUpdate`): a
   pillar strike from the ground. On the first frame it takes the target's position (or, without a
   target, the point 500 ahead of its owner along (cos, 0, sin) of the owner's heading rot.y, w = 0,
   with the owner's position as fallback), raises the ray origin by 1000 and casts down
   (`CollisionRaycastPoint`); its position becomes the hit point, or the fallback (target / owner
   position) on a miss, and is copied to its effect's position before the effect is released. From
   age 26 to 64 it sweeps a vertical 1200-unit segment upwards from its position for hits
   (`BtlAttackSweepHit`, hit kind 0x3f); after age 64 it ends (`BtlAttackEnd`). */
void BtlAttackType1CUpdate(BtlAttack *self)
{
    float origin[4];
    float fallback[4];
    float hit[4];
    float up[4];
    const float *src;
    BtlBakugan *owner;
    float angle;

    if (self->age == 0) {
        BtlBakugan *target = (BtlBakugan *)BtlFindBakuganById(self->targetId);

        if (target != NULL) {
            origin[0] = target->base.pos[0];
            origin[1] = target->base.pos[1];
            origin[2] = target->base.pos[2];
            origin[3] = target->base.pos[3];
            fallback[0] = target->base.pos[0];
            fallback[1] = target->base.pos[1];
            fallback[2] = target->base.pos[2];
            fallback[3] = target->base.pos[3];
        } else {
            owner = self->owner;
            angle = owner->base.rot[1];
            /* vrot [C,0,S,0] scaled by 500 (xyz), then + owner pos (xyz); w stays 0 */
            origin[0] = __builtin_cosf(angle) * 500.0f + owner->base.pos[0];
            origin[1] = 0.0f * 500.0f + owner->base.pos[1];
            origin[2] = __builtin_sinf(angle) * 500.0f + owner->base.pos[2];
            origin[3] = 0.0f;
            owner = self->owner;
            fallback[0] = owner->base.pos[0];
            fallback[1] = owner->base.pos[1];
            fallback[2] = owner->base.pos[2];
            fallback[3] = owner->base.pos[3];
        }
        origin[1] = origin[1] + 1000.0f;
        if (CollisionRaycastPoint(origin, hit) != 0) {
            src = hit;
        } else {
            src = fallback;
        }
        self->pos[0] = src[0];
        self->pos[1] = src[1];
        self->pos[2] = src[2];
        self->pos[3] = src[3];
        ((GfxEffect *)self->effect)->pos[0] = self->pos[0];
        ((GfxEffect *)self->effect)->pos[1] = self->pos[1];
        ((GfxEffect *)self->effect)->pos[2] = self->pos[2];
        ((GfxEffect *)self->effect)->pos[3] = self->pos[3];
        self->effect = NULL;
        return;
    }
    if (self->age > 64) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age > 25) {
        up[0] = 0.0f;
        up[2] = 0.0f;
        up[1] = 1200.0f;
        up[3] = 0.0f;
        BtlAttackSweepHit(self->radius, self, self->pos, up, 0x3f, 3, 0, 0x31bf337e);
    }
}
