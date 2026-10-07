// bdc 0x0887df28 BtlAttackType21Update
#include "bdc.h"

/* Per-frame handler of attack type 0x21 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   on the first frame drops onto the floor under its target (or owner) like
   `BtlAttackType1CUpdate`: origin = target position, or 500 ahead of the owner along
   (cos, 0, sin) of the owner's rot.y (origin.w = 0); fallback = target / owner position; origin
   raised by 1000 and cast down (`CollisionRaycastPoint`); its position becomes the hit point or
   the fallback, is copied to its effect's position and the effect is released. Then, while
   age < 190, it looks up its attached effect 0x20c (`GfxEffectFindAttached` on its position); if
   found, binds it to the owner (pointer and id), sets auxVec to its position raised by
   `effect.vec1d0[2] × 18 − 70` and, on odd frames, spawns effect 0xc1 attached to auxVec. At age
   190 it ends (`BtlAttackEnd`). */

void BtlAttackType21Update(BtlAttack *self)
{
    float origin[4];
    float fallback[4];
    float hit[4];
    int i;

    if (self->age == 0) {
        BtlBakugan *target = (BtlBakugan *)BtlFindBakuganById(self->targetId);

        if (target != NULL) {
            for (i = 0; i < 4; i++) {
                origin[i] = target->base.pos[i];
            }
            for (i = 0; i < 4; i++) {
                fallback[i] = target->base.pos[i];
            }
        } else {
            float yaw = self->owner->base.rot[1];

            origin[0] = __builtin_cosf(yaw) * 500.0f;
            origin[1] = 0.0f * 500.0f;
            origin[2] = __builtin_sinf(yaw) * 500.0f;
            origin[3] = 0.0f;
            origin[0] = origin[0] + self->owner->base.pos[0];
            origin[1] = origin[1] + self->owner->base.pos[1];
            origin[2] = origin[2] + self->owner->base.pos[2];
            for (i = 0; i < 4; i++) {
                fallback[i] = self->owner->base.pos[i];
            }
        }
        origin[1] = origin[1] + 1000.0f;
        if (CollisionRaycastPoint(origin, hit) != 0) {
            for (i = 0; i < 4; i++) {
                self->pos[i] = hit[i];
            }
        } else {
            for (i = 0; i < 4; i++) {
                self->pos[i] = fallback[i];
            }
        }
        for (i = 0; i < 4; i++) {
            ((GfxEffect *)self->effect)->pos[i] = self->pos[i];
        }
        self->effect = NULL;
        return;
    }
    if (self->age < 190) {
        GfxEffect *fx = (GfxEffect *)GfxEffectFindAttached(
            (GfxEffectMgr *)g_btlAttackEffectMgr, 0x20c, self->pos);
        BtlBakugan *owner;

        if (fx == NULL) {
            return;
        }
        owner = self->owner;
        fx->ownerBakugan = owner;
        if (owner != NULL) {
            fx->ownerId = owner->base.base.id;
        }
        for (i = 0; i < 4; i++) {
            self->auxVec[i] = self->pos[i];
        }
        self->auxVec[1] = self->auxVec[1] + (fx->vec1d0[2] * 18.0f - 70.0f);
        if ((self->age & 1) != 0) {
            GfxEffectSpawnAttached((GfxEffectMgr *)g_btlAttackEffectMgr, 0xc1, self->auxVec);
        }
        return;
    }
    BtlAttackEnd(self);
}
