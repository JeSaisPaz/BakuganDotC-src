// bdc 0x0885c754 BtlUnitAltState12Update
#include "bdc.h"

/* State-12 (jump) handler override of the unit subclass with vtable `0x08af1c94` (slot `+0x130`),
   a variant of `BtlBakuganState12Update` whose take-off and double jump depend on
   `stats->dashMode`. With command bit 4 the take-off speed is `stats->dashJumpSpeed` and the
   horizontal cap `BtlBakuganGetScaledStat44`, otherwise `stats->jumpSpeed` and
   `BtlBakuganGetScaledStat38`.
   Take-off (motion 0x19 playing): once the motion is 80% done (dashMode 1; then motion 0x1a) or
   40% done (other modes; then motion 0) it plays kind sound 6 in stage water (else 5) plus sound 7
   when the signed low nibble of `stats->stepSoundMode` is 1, sets velocity.xz to the input heading
   x cap when command bit 1 is set, sets velocity.y to the take-off speed (x 0.8 when `slowWalk` is
   set and virtual entry 20 does not return 2) and `airborneFrames` to 4. Then without command
   bit 4 velocity.xz is scaled by 0.8; with it, a horizontal speed above the cap eases toward it
   (rate 0.2).
   Airborne (any other motion): dashMode 1 switches motion 0x1a to 0x1b while falling (sound 4 when
   it starts), other modes play motion 0 while falling; turns toward the target
   (`BtlBakuganTurnTowardTarget`), or else toward the input heading at rate 0.3. Command bit 1
   adds the input heading to velocity.xz; a horizontal speed above the cap eases toward it
   (rate 0.15). velocity.y loses `gravity` and the unit moves by `BtlBakuganApplyVelocity`. When
   the unit's bottom plus 0.8 x velocity.y is below `groundPoint.y`, or `stateFlags` bit 31 is set,
   it lands (state 0xe). Otherwise: attack command with energy (`BtlCombatHasEnergy`) ->
   `BtlBakuganStartAttackOrArt`; command bit 4 with energy -> state 2; with energy and dashMode 1,
   below 0.5 x take-off speed and command bit 2 -> state 0xc again with the motion moved to 30% of
   its length; with energy, another dashMode and command bit 0x200 -> state 0xd.
   Every heading vector is (cos, 0, sin) of the heading; every rescale multiplies by
   1/sqrt(squared length), taken as 0 for a zero vector. */

void BtlUnitAltState12Update(BtlBakugan *self)
{
    float flat[3];
    float vel[4];
    float heading;
    float sq;
    float k;
    const VtblEntry *entry;
    float jump;
    float cap;
    float len;
    float vy;
    int faced;
    int reached;
    u8 damp;

    if ((self->commands & 4) == 0) {
        jump = self->combat.stats->jumpSpeed;
        cap = BtlBakuganGetScaledStat38(self);
    } else {
        jump = self->combat.stats->dashJumpSpeed;
        cap = BtlBakuganGetScaledStat44(self);
    }
    if (BtlBakuganIsMotion(self, 0x19) != 0) {
        if (self->combat.stats->dashMode == 1) {
            reached = GfxModelMotionReached(&self->base, 0.800000012f);
            if (reached) {
                BtlBakuganPlayMotion(0.100000001f, self, 0x1a, 1, 0);
            }
        } else {
            reached = GfxModelMotionReached(&self->base, 0.400000006f);
            if (reached) {
                BtlBakuganPlayMotion(0.100000001f, self, 0, 1, 0);
            }
        }
        if (reached) {
            if (BtlBakuganIsInStageWater(self) != 0) {
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
    if (self->combat.stats->dashMode == 1) {
        if (BtlBakuganIsMotion(self, 0x1a) != 0 && self->base.velocity[1] < 0.0f &&
            BtlBakuganPlayMotion(0.200000003f, self, 0x1b, 1, 0) != 0) {
            BtlBakuganPlayKindSound(self, 4, 0, 0);
        }
    } else if (self->base.velocity[1] < 0.0f) {
        BtlBakuganPlayMotion(0.200000003f, self, 0, 1, 0);
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
    self->base.velocity[1] = self->base.velocity[1] - self->gravity;
    vel[0] = self->base.velocity[0];
    vel[1] = self->base.velocity[1];
    vel[2] = self->base.velocity[2];
    vel[3] = self->base.velocity[3];
    BtlBakuganApplyVelocity(self, vel);
    if (self->base.pos[1] + self->combat.stats->hoverHeight +
                self->base.velocity[1] * 0.800000012f < self->groundPoint[1] ||
        (self->stateFlags & 0x80000000) == 0x80000000) {
        BtlBakuganSetState(self, 0xe, 0);
        return;
    }
    if ((self->commands & 0x10) != 0 && BtlCombatHasEnergy(&self->combat) != 0) {
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
        if (!(self->base.velocity[1] < jump * 0.5f)) {
            return;
        }
        if ((self->commands & 2) == 0) {
            return;
        }
        BtlBakuganSetState(self, 0xc, 0);
        GfxModelSwapMotionFrame(&self->base, GfxModelGetMotionEnd(&self->base) * 0.300000012f);
        return;
    }
    if ((self->commands & 0x200) != 0) {
        BtlBakuganSetState(self, 0xd, 0);
    }
}
