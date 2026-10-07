// bdc 0x08881458 BtlAttackType5CUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x5c (entry 92 of the attack handler table run by
   `BtlAttackUpdate`). Frame 0 clears paramF2 and plays sound 0xa00029. From frame 1 it zeroes
   the y velocity and, after frame 20, decays paramF1 by 0.9 each frame. Up to frame 40 it runs
   the shared guided step `BtlAttackUpdateGuided` (turn 0.1, speed paramF2, hit kind 0x7f, hit
   and cancel effect 0xb0). After frame 40 it spawns effect 0xf6 at its position tagged with the
   owner Bakugan and its id, ends itself, launches 8 child attacks of type 0x5e (`BtlAttackCtor` +
   `BtlAttackLaunchAuto`) in horizontal directions 45 degrees apart (a failed allocation launches
   with a NULL attack), and plays sound 0x200098. */

void BtlAttackType5CUpdate(BtlAttack *self)
{
    GfxEffect *effect;
    BtlBakugan *owner;
    BtlAttack *child;
    BtlAttack *launched;
    bool fromLow;
    float dir[4] __attribute__((aligned(16)));
    float angle;
    float c;
    float s;
    float len2;
    float k;
    int i;

    if (self->age == 0) {
        self->paramF2 = 0.0f;
        BtlAttackPlaySound(self, 0xa00029, NULL, 0, 0);
    } else {
        self->vel[1] = 0.0f;
        if (self->age > 20) {
            self->paramF1 = self->paramF1 * 0.899999976f;
        }
        if (self->age > 40) {
            effect = (GfxEffect *)GfxEffectSpawn(g_btlAttackEffectMgr, 0xf6, self->pos);
            owner = self->owner;
            effect->ownerBakugan = owner;
            if (owner != NULL) {
                effect->ownerId = owner->base.base.id;
            }
            BtlAttackEnd(self);
            for (i = 0; i < 8; i++) {
                launched = NULL;
                MemLock();
                fromLow = MemIsAllocFromLow();
                MemSetAllocFromLow(true);
                child = MemAlloc(0x160, NULL, 0);
                MemSetAllocFromLow(fromLow);
                MemUnlock();
                if (child != NULL) {
                    BtlAttackCtor(child, self->owner, 0x5e);
                    launched = child;
                }
                angle = (float)i * 0.785398006f;
                /* dir.xyz = (cos, 0, sin) of angle, normalised with a zero length giving scale 0
                   and each lane clamped to [-1, 1] (vpfxd). The w lane is masked: dir[3] keeps a
                   stale VFPU value and is left unset. */
                c = __builtin_cosf(angle);
                s = __builtin_sinf(angle);
                len2 = c * c + 0.0f * 0.0f + s * s;
                k = VfRsq(len2);
                if (len2 == 0.0f) {
                    k = 0.0f;
                }
                dir[0] = VfSat1(c * k);
                dir[1] = VfSat1(0.0f * k);
                dir[2] = VfSat1(s * k);
                BtlAttackLaunchAuto(launched, self->pos, dir, NULL);
            }
            BtlAttackPlaySound(self, 0x200098, NULL, 0, 0);
            return;
        }
    }
    BtlAttackUpdateGuided(0.100000001f, self->paramF2, self, 0x7f, 0xb0, 0xb0);
}
