// bdc 0x088a4cac ActorStageObjEggCrystalState01Appear
#include "bdc.h"

/* State 1 of the egg crystal. Step 0 spawns the ground effect 0x36 under it (`GfxEffectSpawn`),
   sets bit 1 of the companion unit's collider flags, zeroes the alpha and waits 2 frames; step 3
   then pulses the alpha (`0.65 + 0.35*sin(t*pi*0.03)`) with translucent materials each frame,
   clearing the collider bit again once the alpha is at least 0.2, and advances the step once it
   reaches 0.95. Any step past 3 (or below 0) clears the collider bit, sets full alpha and switches
   to state 2 (`ActorStageObjEggCrystalSetState`). */

void ActorStageObjEggCrystalState01Appear(ActorStageObjEggCrystal *self)
{
    float ground[4];
    float alpha;
    int t;

    switch (self->step) {
    case 0:
        CollisionFindGroundPoint(ground, &self->base.base.data->rootMatrix[12], 0x3fbf2500);
        GfxEffectSpawn(g_btlUnitEffectMgr, 0x36, ground);
        if (self->unit != NULL) {
            ((BtlTargetPoint *)self->unit)->base.collider0->flags |= 2;
        }
        self->base.fade = 0.0f;
        self->timer = 2;
        self->step = self->step + 1;
        /* fallthrough */
    case 1:
        self->timer = self->timer - 1;
        if (self->timer > 0) {
            return;
        }
        self->step = self->step + 1;
        /* fallthrough */
    case 2:
        self->timer = 0;
        self->step = self->step + 1;
        /* fallthrough */
    case 3:
        GfxModelForEachMaterial(&self->base.base, (void *)ActorStageObjEggCrystalMaterialSetTranslucent,
                                NULL);
        t = self->timer;
        self->timer = t + 1;
        alpha = __builtin_sinf((float)t * 3.1415927f * 0.03f) * 0.35f + 0.65f;
        self->base.fade = alpha;
        if (!(alpha < 0.95f)) {
            self->step = self->step + 1;
        } else if (!(alpha < 0.2f) && self->unit != NULL) {
            ((BtlTargetPoint *)self->unit)->base.collider0->flags &= ~2u;
        }
        return;
    default:
        if (self->unit != NULL) {
            ((BtlTargetPoint *)self->unit)->base.collider0->flags &= ~2u;
        }
        self->base.fade = 1.0f;
        ActorStageObjEggCrystalSetState(self, 2);
        return;
    }
}
