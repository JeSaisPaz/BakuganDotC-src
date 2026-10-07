// bdc 0x088804bc BtlAttackType2CUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x2c (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   on the first frame (after a VFPU distance to the owner whose result is discarded) turns
   the side parameter `paramF2` (truncated: -1, 0, 1) into a lateral offset +2400, 0 or -2400
   (other values are kept). On later frames it zeroes `vel[1]`; after 30 frames the speed factor
   `paramF1` decays by 7 %/frame; after 90 frames it bursts (effect 0xb0 on
   `g_btlAttackEffectMgr` at `pos`, bound to the owner) and ends (`BtlAttackEnd`). Unless it
   burst, runs `BtlAttackUpdateGuided``(0.1, paramF2, self, 0x4f, 0xb0, 0xb0)`. */
void BtlAttackType2CUpdate(BtlAttack *self)
{
    s32 side;
    GfxEffect *burst;
    BtlBakugan *owner;

    if (self->age == 0) {
        side = (s32)self->paramF2;
        if (side < 0) {
            if (side >= -1) {
                self->paramF2 = 2400.0f;
            }
        } else if (side <= 0) {
            self->paramF2 = 0.0f;
        } else if (side < 2) {
            self->paramF2 = -2400.0f;
        }
    } else {
        self->vel[1] = 0.0f;
        if (self->age > 30) {
            self->paramF1 = self->paramF1 * 0.930000007f;
        }
        if (self->age > 90) {
            burst = (GfxEffect *)GfxEffectSpawn(g_btlAttackEffectMgr, 0xb0, self->pos);
            owner = self->owner;
            burst->ownerBakugan = owner;
            if (owner != NULL) {
                burst->ownerId = owner->base.base.id;
            }
            BtlAttackEnd(self);
            return;
        }
    }
    BtlAttackUpdateGuided(0.100000001f, self->paramF2, self, 0x4f, 0xb0, 0xb0);
}
