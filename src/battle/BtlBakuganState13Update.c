// bdc 0x08872c38 BtlBakuganState13Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 13 (`state`), vtable slot `+0x138` called through
   `BtlBakuganRunState`: turns toward the target, applies the state-2 energy cost and sets state
   flag 0x800000. Without move command (bit 1) it decays `dashSpeed` and the horizontal velocity by
   0.9 and loops motion 0xd. With it, `dashSpeed` eases toward scaled stat 0x60 (rate 0.2); facing
   a target it loops the directional motion `0xe + slot` (`BtlBakuganGetTargetDirSlot`) at full
   speed, otherwise it loops motion 0xe, turns toward the input heading and scales the speed by
   how far it still has to turn (1 within 6 degrees, 0 from 126 degrees); it runs virtual entry 6 with
   1.3 and sets the horizontal velocity from the input heading: x = cos(heading) x speed,
   z = sin(heading) x speed. Then: attack command -> `BtlBakuganStartAttackOrArt`;
   command bit 4 with energy -> state 2; without command bit 0x200 or with no energy -> state 0x17;
   otherwise it rises: vertical speed += scaled stat 0x68 (falling) or 0x64, capped at scaled stat
   0x6c, and while rising it is damped by -0.01 x its height above `stats->riseCeiling + groundHeight`
   when that height is above -100. Always counts statistic 0xd when a stats object is linked. */

void BtlBakuganState13Update(BtlBakugan *self)
{
    const VtblEntry *entry;
    float heading;
    float factor;
    float speed;
    float vy;
    float cap;
    float height;
    int faced;

    faced = BtlBakuganTurnTowardTarget(self, NULL);
    BtlBakuganApplyStateEnergy(self, 2, 0);
    self->stateFlags |= 0x800000;
    if ((self->commands & 1) == 0) {
        self->dashSpeed = self->dashSpeed * 0.899999976f;
        self->base.velocity[0] = self->base.velocity[0] * 0.899999976f;
        self->base.velocity[2] = self->base.velocity[2] * 0.899999976f;
        BtlBakuganPlayMotion(0.200000003f, self, 0xd, 1, 0);
    } else {
        speed = self->dashSpeed;
        self->dashSpeed = speed + (BtlBakuganGetScaledStat60(self) - self->dashSpeed) *
                                      0.200000003f;
        if (faced != 0) {
            BtlBakuganPlayMotion(0.200000003f, self, BtlBakuganGetTargetDirSlot(self) + 0xe, 1, 0);
            factor = 1.0f;
        } else {
            BtlBakuganPlayMotion(0.200000003f, self, 0xe, 1, 0);
            factor = ABS(BtlBakuganTurnToward(self->input->heading, 0.300000012f, 0.0f, self)) +
                     0.942477882f;
            if (!(factor <= 3.14159274f)) {
                factor = 3.14159274f;
            }
            factor = (3.14159274f - factor) * 0.477464825f;
            if (!(factor <= 1.0f)) {
                factor = 1.0f;
            }
        }
        entry = &((const VtblEntry *)self->base.base.vtable)[6];
        ((float (*)(float, void *))entry->fn)(1.29999995f, (u8 *)self + entry->delta);
        heading = self->input->heading;
        speed = self->dashSpeed * factor;
        self->base.velocity[0] = __builtin_cosf(heading) * speed;
        self->base.velocity[2] = __builtin_sinf(heading) * speed;
    }
    if (BtlBakuganHasAttackCommand(self) != 0) {
        BtlBakuganStartAttackOrArt(self, 0);
    } else if ((self->commands & 4) != 0 && BtlCombatHasEnergy(&self->combat) != 0) {
        BtlBakuganSetState(self, 2, 0);
    } else if ((self->commands & 0x200) == 0 || BtlCombatIsEnergyDepleted(&self->combat) != 0) {
        BtlBakuganSetState(self, 0x17, 0);
    } else {
        if (self->base.velocity[1] < 0.0f) {
            vy = BtlBakuganGetScaledStat68(self);
        } else {
            vy = BtlBakuganGetScaledStat64(self);
        }
        vy = self->base.velocity[1] + vy;
        cap = BtlBakuganGetScaledStat6C(self);
        if (cap < vy) {
            vy = cap;
        }
        self->base.velocity[1] = vy;
        if (!(vy <= 0.0f)) {
            height = self->base.pos[1] - (self->combat.stats->riseCeiling + self->groundHeight);
            if (!(height <= -100.0f)) {
                self->base.velocity[1] = self->base.velocity[1] * (height * -0.00999999978f);
            }
        }
    }
    if (self->stats != NULL) {
        BtlStatsAddCounter(self->stats, 0xd, 1);
    }
}
