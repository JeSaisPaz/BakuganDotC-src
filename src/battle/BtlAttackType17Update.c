// bdc 0x0887d1a4 BtlAttackType17Update
#include "bdc.h"

/* Per-frame handler of attack type 0x17 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   on the first frame turns the side parameter `paramF2` (truncated: 0, 1, 2) into a lateral offset
   0, +2400 or -2400 (other values are kept). On later frames it zeroes `vel[1]`; after 30 frames
   the speed factor `paramF1` decays by 7 %/frame; after 90 frames it bursts (effect 0xb0 on
   `g_btlAttackEffectMgr` at `pos`, bound to the owner) and ends (`BtlAttackEnd`). Unless it
   burst, runs `BtlAttackUpdateGuided``(0.1, paramF2, self, 0x3a, 0xb0, 0xb0)`. */

void BtlAttackType17Update(BtlAttack *self)
{
    s32 age = self->age;
    s32 side;
    GfxEffect *burst;
    BtlBakugan *owner;

    if (age == 0) {
        side = (s32)self->paramF2;
        if (side <= 0) {
            if (side >= 0) {
                self->paramF2 = 0.0f;
            }
        } else if (side < 2) {
            self->paramF2 = 2400.0f;
        } else if (side < 3) {
            self->paramF2 = -2400.0f;
        }
    } else {
        self->vel[1] = 0.0f;
        if (age > 30) {
            self->paramF1 = self->paramF1 * 0.930000007f;
        }
        if (age > 90) {
            burst = GfxEffectSpawn(g_btlAttackEffectMgr, 0xb0, self->pos);
            owner = self->owner;
            burst->ownerBakugan = owner;
            if (owner != NULL) {
                burst->ownerId = owner->base.base.id;
            }
            BtlAttackEnd(self);
            return;
        }
    }
    BtlAttackUpdateGuided(0.100000001f, self->paramF2, self, 0x3a, 0xb0, 0xb0);
}
