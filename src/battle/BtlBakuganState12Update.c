// bdc 0x08875104 BtlBakuganState12Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 12 (`state`), vtable slot `+0x130` called through
   `BtlBakuganRunState`: the jump. With command bit 4 (dash) held the take-off speed is
   `stats->dashJumpSpeed` and the horizontal speed cap `BtlBakuganGetScaledStat44`, otherwise
   `stats->jumpSpeed` and `BtlBakuganGetScaledStat38`.
   Take-off (motion 0x19 playing): an attack command starts `BtlBakuganStartAttackOrArt`; once
   the motion is 90% done it plays motion 0x1a, kind sound 6 in stage water with `inWater` set
   (else 5) plus sound 7 when the signed low nibble of `stats->stepSoundMode` is 1, sets velocity.xz
   to the input heading x cap when command bit 1 is set, sets velocity.y to the take-off speed
   (x 0.8 when `slowWalk` is set and virtual entry 20 does not return 2) and `airborneFrames` to 4.
   Then without command bit 4 velocity.xz is scaled by 0.8; with it, a horizontal speed above the
   cap eases toward it (rate 0.2).
   Airborne (any other motion): playing motion 0x1a while falling switches to motion 0x1b (sound 4
   when it starts); turns toward the target (`BtlBakuganTurnTowardTarget`), or else toward the
   input heading at rate 0.3. Command bit 1 adds the unit input heading to velocity.xz; a
   horizontal speed above the cap eases toward it (rate 0.15). velocity.y loses `gravity` and, while
   rising, is damped by -0.01 x the height above `stats->riseCeiling + groundHeight` when that is
   between -100 and 0. Counts `airborneFrames` and moves by `BtlBakuganApplyVelocity`. When the
   unit's bottom plus 0.8 x velocity.y is below `groundPoint.y`, or `stateFlags` bit 31 is set, it
   lands: `BtlBakuganSpawnWaterEffects` at ground height, state 0xe, `airborneFrames` = 0.
   Otherwise: attack command -> `BtlBakuganStartAttackOrArt`; command bit 4 with energy -> state
   2; with energy and `stats->dashMode` 1, falling below 0.8 x take-off speed and command bit 2 it
   double-jumps (state-1 energy, motion 0x19 from 30% of its length, statistic 0xc); with another
   dashMode, below 0.3 x take-off speed and command bit 0x200 -> state 0xd.
   Every heading vector is (cos, 0, sin) of the heading; every rescale multiplies by
   1/sqrt(squared length), taken as 0 for a zero vector. */

void BtlBakuganState12Update(BtlBakugan *self)
{
    float flat[3];
    float vel[4];
    const VtblEntry *entry;
    float jump;
    float cap;
    float len;
    float sq;
    float k;
    float heading;
    float vy;
    float height;
    float saved;
    float ground;
    int faced;
    u8 damp;

    if ((self->commands & 4) == 0) {
        jump = self->combat.stats->jumpSpeed;
        cap = BtlBakuganGetScaledStat38(self);
    } else {
        jump = self->combat.stats->dashJumpSpeed;
        cap = BtlBakuganGetScaledStat44(self);
    }
    if (BtlBakuganIsMotion(self, 0x19) != 0) {
        if (BtlBakuganHasAttackCommand(self) != 0) {
            BtlBakuganStartAttackOrArt(self, 0);
        } else if (GfxModelMotionReached(&self->base, 0.899999976f)) {
            BtlBakuganPlayMotion(0.100000001f, self, 0x1a, 1, 0);
            if (BtlBakuganIsInStageWater(self) != 0 && self->inWater != 0) {
                BtlBakuganPlayKindSound(self, 6, 0, 0);
            } else {
                BtlBakuganPlayKindSound(self, 5, 0, 0);
            }
            if ((s8)(((self->combat.stats->stepSoundMode & 0xf) ^ 8) - 8) == 1) {
                BtlBakuganPlayKindSound(self, 7, 0, 0);
            }
            if ((self->commands & 1) != 0) {
                /* velocity.xz = (cos, sin)(heading) * cap */
                heading = self->input->heading;
                self->base.velocity[0] = __builtin_cosf(heading) * cap;
                self->base.velocity[2] = __builtin_sinf(heading) * cap;
            }
            self->base.velocity[1] = jump;
            damp = 0;
            if (self->slowWalk != 0) {
                entry = &((const VtblEntry *)self->base.base.vtable)[20];
                if (((s32 (*)(void *))entry->fn)((u8 *)self + entry->delta) != 2) {
                    damp = 1;
                }
            }
            if (damp != 0) {
                self->base.velocity[1] = self->base.velocity[1] * 0.800000012f;
            }
            self->airborneFrames = 4;
        }
        if ((self->commands & 4) == 0) {
            self->base.velocity[0] = self->base.velocity[0] * 0.800000012f;
            self->base.velocity[2] = self->base.velocity[2] * 0.800000012f;
            return;
        }
        /* flat = velocity with y = 0; len = |flat| */
        flat[0] = self->base.velocity[0];
        flat[1] = 0.0f;
        flat[2] = self->base.velocity[2];
        len = __builtin_sqrtf(flat[0] * flat[0] + flat[1] * flat[1] + flat[2] * flat[2]);
        if (cap < len) {
            len = len + (cap - len) * 0.200000003f;
            /* flat = flat / |flat| * len (1/|flat| taken as 0 for a zero vector) */
            sq = flat[0] * flat[0] + flat[1] * flat[1] + flat[2] * flat[2];
            k = (sq == 0.0f) ? 0.0f : VfRsq(sq);
            k = k * len;
            self->base.velocity[0] = flat[0] * k;
            self->base.velocity[2] = flat[2] * k;
        }
        return;
    }
    if (BtlBakuganIsMotion(self, 0x1a) != 0 && self->base.velocity[1] < 0.0f &&
        BtlBakuganPlayMotion(0.200000003f, self, 0x1b, 1, 0) != 0) {
        BtlBakuganPlayKindSound(self, 4, 0, 0);
    }
    faced = BtlBakuganTurnTowardTarget(self, NULL);
    vy = self->base.velocity[1];
    if ((self->commands & 1) != 0) {
        /* velocity.xyz += (cos(heading), 0, sin(heading)) */
        heading = self->input->heading;
        self->base.velocity[0] = self->base.velocity[0] + __builtin_cosf(heading);
        self->base.velocity[2] = self->base.velocity[2] + __builtin_sinf(heading);
    }
    self->base.velocity[1] = 0.0f;
    len = __builtin_sqrtf(self->base.velocity[0] * self->base.velocity[0] +
                          self->base.velocity[1] * self->base.velocity[1] +
                          self->base.velocity[2] * self->base.velocity[2]);
    if (cap < len) {
        len = len + (cap - len) * 0.150000006f;
        /* velocity = velocity / |velocity| * len; lane 3 gets the bank zero S713 */
        sq = self->base.velocity[0] * self->base.velocity[0] +
             self->base.velocity[1] * self->base.velocity[1] +
             self->base.velocity[2] * self->base.velocity[2];
        k = (sq == 0.0f) ? 0.0f : VfRsq(sq);
        k = k * len;
        self->base.velocity[0] = self->base.velocity[0] * k;
        self->base.velocity[2] = self->base.velocity[2] * k;
        self->base.velocity[3] = 0.0f;
    }
    self->base.velocity[1] = vy;
    if (faced == 0) {
        BtlBakuganTurnToward(self->input->heading, 0.300000012f, 0.0f, self);
    }
    vy = self->base.velocity[1] - self->gravity;
    self->base.velocity[1] = vy;
    if (!(vy <= 0.0f)) {
        height = self->base.pos[1] - (self->combat.stats->riseCeiling + self->groundHeight);
        if (!(height <= -100.0f) && height < 0.0f) {
            self->base.velocity[1] = self->base.velocity[1] * (height * -0.00999999978f);
        }
    }
    self->airborneFrames = self->airborneFrames + 1;
    vel[0] = self->base.velocity[0];
    vel[1] = self->base.velocity[1];
    vel[2] = self->base.velocity[2];
    vel[3] = self->base.velocity[3];
    BtlBakuganApplyVelocity(self, vel);
    if (self->base.pos[1] + self->combat.stats->hoverHeight +
                self->base.velocity[1] * 0.800000012f < self->groundPoint[1] ||
        (self->stateFlags & 0x80000000) == 0x80000000) {
        ground = self->groundPoint[1];
        saved = self->base.pos[1];
        self->base.pos[1] = ground;
        BtlBakuganSpawnWaterEffects(self);
        self->base.pos[1] = saved;
        BtlBakuganSetState(self, 0xe, 0);
        self->airborneFrames = 0;
        return;
    }
    if (BtlBakuganHasAttackCommand(self) != 0) {
        BtlBakuganStartAttackOrArt(self, 0);
        return;
    }
    if ((self->commands & 4) != 0 && BtlCombatHasEnergy(&self->combat) != 0) {
        BtlBakuganSetState(self, 2, 0);
        return;
    }
    if (BtlCombatHasEnergy(&self->combat) == 0) {
        return;
    }
    if (self->combat.stats->dashMode == 1) {
        if (!(self->base.velocity[1] < jump * 0.800000012f)) {
            return;
        }
        if ((self->commands & 2) == 0) {
            return;
        }
        BtlBakuganApplyStateEnergy(self, 1, 0);
        BtlBakuganPlayMotion(0.100000001f, self, 0x19, 0, 0);
        if (self->stats != NULL) {
            BtlStatsAddCounter(self->stats, 0xc, 1);
        }
        GfxModelSwapMotionFrame(&self->base, GfxModelGetMotionEnd(&self->base) * 0.300000012f);
        return;
    }
    if (!(self->base.velocity[1] < jump * 0.300000012f)) {
        return;
    }
    if ((self->commands & 0x200) == 0) {
        return;
    }
    BtlBakuganSetState(self, 0xd, 0);
}
