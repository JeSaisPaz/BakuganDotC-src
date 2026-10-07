// bdc 0x08871d88 BtlBakuganDashStep
#include "bdc.h"

/* Movement step of the energy-consuming dash (states 2 and 10): counts the dash in the battle stats
   (`BtlStatsAddCounter` 0xe), drains the dash energy (`BtlBakuganApplyStateEnergy` 3), marks
   the unit flag 0x1000000, and for the local player spawns effect 0x1d twice along the normalised
   velocity (`GfxEffectSpawnDirected`). The dash speed decays by `BtlBakuganGetStat54` x motion
   time scale down to `BtlBakuganGetScaledStat4C`; the vertical velocity follows the unit's
   `moveStyle` (0: climb toward the cruise height, 1: glide with gravity damping) and `dashLift`
   decays by 0.8. With a target (`BtlBakuganGetTarget`) `dashHeading` turns toward the input
   heading (command bit 0, `BtlBakuganGetTargetDirection`) or toward the target
   (`BtlBakuganClassifyTargetDirection`), optionally adding a pitch toward it; without one the
   unit turns toward the input heading and banks (`dashBank`). The horizontal velocity is set to
   `dashSpeed` along the heading and the tilt quaternion `orient` is rebuilt. Returns the motion
   id: 5 (target ahead/behind or no target), 7 (direction 2), 6 (direction 3), plus 0xd when
   `BtlBakuganIsAirborne` (with height check).
   VFPU bank constants S703 (2/pi, so vrot/vcos take radians) and S713 (0: normalise fallback for
   a zero vector, and the w lane of the normalised vectors) are literals. */

#define DASH_PI      3.14159274f
#define DASH_TWO_PI  6.28318548f
#define DASH_INV_PI  0.318309873f

/* vdot/vrsq normalise of xyz: a zero vector scales by 0 (S713), each lane saturated to [-1, 1]
   (vpfxd), lane 3 is C710's w, the bank's S713 (0) */
static void DashNormalize(float *out, const float *v)
{
    float lenSq;
    float k;

    lenSq = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    k = VfRsq(lenSq);
    if (lenSq == 0.0f) {
        k = 0.0f;
    }
    out[0] = VfSat1(v[0] * k);
    out[1] = VfSat1(v[1] * k);
    out[2] = VfSat1(v[2] * k);
    out[3] = 0.0f;
}

/* vrot [C,0,-S,0] / [S,0,C,0] about the yaw (radians), then the normalise, in place */
static void DashRotateOrient(float *orient, float yaw)
{
    float c;
    float s;
    float rot[4];

    c = __builtin_cosf(yaw);
    s = __builtin_sinf(yaw);
    rot[0] = orient[0] * c + orient[1] * 0.0f + orient[2] * -s;
    rot[1] = orient[1];
    rot[2] = orient[0] * s + orient[1] * 0.0f + orient[2] * c;
    rot[3] = orient[3];
    orient[0] = rot[0];
    orient[1] = rot[1];
    orient[2] = rot[2];
    orient[3] = rot[3];
    DashNormalize(orient, orient);
}

int BtlBakuganDashStep(BtlBakugan *self)
{
    int result;
    int dir;
    float speed;
    float decay;
    float minSpeed;
    float height;
    float vy;
    float lift;
    float heading;
    float diff;
    float wrapped;
    int style;
    float turn;
    float rate;
    float turned;
    float bank;
    float dot;
    float align;
    float cosTurn;
    float limit;
    float *vel;
    BtlBakugan *target;
    float effectDir[4];
    float velDir[4];
    float toTarget[4];
    float moveDir[4];
    float bankDir[4];

    result = 5;
    vel = self->base.velocity;
    if (self->stats != NULL) {
        BtlStatsAddCounter(self->stats, 0xe, 1);
    }
    BtlBakuganApplyStateEnergy(self, 3, 0);
    self->stateFlags |= 0x1000000;
    if (BtlBakuganIsLocalPlayer(self)) {
        DashNormalize(effectDir, vel);
        GfxEffectSpawnDirected(g_worldEffectMgr, 0x1d, self->base.pos, effectDir);
        GfxEffectSpawnDirected(g_worldEffectMgr, 0x1d, self->base.pos, effectDir);
    }

    /* dash speed decays to the scaled stat 4C */
    speed = self->dashSpeed;
    decay = BtlBakuganGetStat54(self);
    speed = speed - decay * GfxGetMotionTimeScale();
    minSpeed = BtlBakuganGetScaledStat4C(self);
    if (!(minSpeed <= speed)) {
        speed = minSpeed;
    }
    self->dashSpeed = speed;

    /* vertical velocity by move style */
    style = self->combat.stats->moveStyle;
    if (style == 0) {
        height = self->groundPoint[1] + self->combat.stats->cruiseHeight - self->base.pos[1];
        if (height < 0.0f) {
            height = 0.0f;
        }
        self->base.velocity[1] = self->gravity + height * 0.300000012f + self->dashLift;
    } else if (style == 1) {
        if (self->combat.stats->cruiseHeight == 0.0f) {
            vy = self->gravity * 0.699999988f + self->dashLift * 0.200000003f + self->base.velocity[1];
            self->base.velocity[1] = vy;
            if (!(vy <= 0.0f)) {
                vy = 0.0f;
            }
            self->base.velocity[1] = vy;
        } else {
            height = self->groundPoint[1] + self->combat.stats->cruiseHeight - self->base.pos[1];
            if (height < 0.0f) {
                self->base.velocity[1] =
                    self->gravity * 0.5f + self->dashLift * 0.200000003f + self->base.velocity[1];
            } else {
                height = height * 0.200000003f;
                if (!(height <= 2.0f)) {
                    height = 2.0f;
                }
                self->base.velocity[1] = self->gravity * 0.5f + height +
                                         self->dashLift * 0.200000003f + self->base.velocity[1];
            }
            vy = self->base.velocity[1];
            self->base.velocity[1] = vy + (self->gravity * 0.699999988f - vy) * 0.100000001f;
        }
    }
    lift = self->dashLift;
    self->dashLift = lift * 0.800000012f;

    if (BtlBakuganGetTarget(self) != NULL) {
        target = (BtlBakugan *)BtlBakuganGetTarget(self);
        if (self->commands & 1) {
            /* steer by the input heading */
            diff = self->dashHeading - self->input->heading;
            wrapped = diff - (float)(int)(diff * DASH_INV_PI) * DASH_TWO_PI;
            if (wrapped < 0.0f) {
                wrapped += DASH_TWO_PI;
            }
            if (wrapped < DASH_PI) {
                turn = -wrapped;
            } else {
                turn = DASH_TWO_PI - wrapped;
            }
            heading = self->dashHeading;
            rate = BtlBakuganGetScaledStat50(self);
            heading = heading + turn * rate;
            self->dashHeading = heading;
            if (!(heading <= DASH_PI)) {
                self->dashHeading = self->dashHeading - DASH_TWO_PI;
            } else if (self->dashHeading <= -DASH_PI) {
                self->dashHeading = self->dashHeading + DASH_TWO_PI;
            }
            dir = BtlBakuganGetTargetDirection(self);
        } else {
            /* steer toward the target */
            heading = self->dashHeading;
            diff = heading - atan2f(target->base.pos[2] - self->base.pos[2],
                                    target->base.pos[0] - self->base.pos[0]);
            wrapped = diff - (float)(int)(diff * DASH_INV_PI) * DASH_TWO_PI;
            if (wrapped < 0.0f) {
                wrapped += DASH_TWO_PI;
            }
            if (wrapped < DASH_PI) {
                turn = -wrapped;
            } else {
                turn = DASH_TWO_PI - wrapped;
            }
            heading = self->dashHeading;
            rate = BtlBakuganGetScaledStat50(self);
            heading = heading + turn * rate * 0.800000012f;
            self->dashHeading = heading;
            if (!(heading <= DASH_PI)) {
                self->dashHeading = self->dashHeading - DASH_TWO_PI;
            } else if (self->dashHeading <= -DASH_PI) {
                self->dashHeading = self->dashHeading + DASH_TWO_PI;
            }
            dir = BtlBakuganClassifyTargetDirection(self->dashHeading, self);
            if (self->combat.stats->dashMode != 1) {
                /* pitch toward the target: dot of the velocity and target directions */
                toTarget[0] = target->base.pos[0] - self->base.pos[0];
                toTarget[1] = target->base.pos[1] - self->base.pos[1];
                toTarget[2] = target->base.pos[2] - self->base.pos[2];
                toTarget[3] = target->base.pos[3];
                DashNormalize(toTarget, toTarget);
                DashNormalize(velDir, vel);
                dot = velDir[0] * toTarget[0] + velDir[1] * toTarget[1] + velDir[2] * toTarget[2];
                align = (dot + 1.0f) * 0.600000024f;
                /* vcos.s of |turn| x S703 (2/pi): the cosine of |turn| radians */
                cosTurn = __builtin_cosf(__builtin_fabsf(turn));
                vy = self->base.velocity[1] + cosTurn * toTarget[1] * self->dashSpeed * align;
                self->base.velocity[1] = vy;
                limit = self->combat.stats->jumpSpeed * 0.699999988f;
                if (!(vy <= limit)) {
                    vy = limit;
                } else if (vy < -limit) {
                    vy = -limit;
                }
                self->base.velocity[1] = vy;
            }
        }
        switch (dir) {
        case 0:
        case 1:
            result = 5;
            break;
        case 2:
            result = 7;
            break;
        case 3:
            result = 6;
            break;
        }
        BtlBakuganTurnToward(self->dashHeading, 0.300000012f, 0.0f, self);
        vel[0] = __builtin_cosf(self->dashHeading) * self->dashSpeed;
        vel[2] = __builtin_sinf(self->dashHeading) * self->dashSpeed;
        self->stateFlags |= 1;
        DashNormalize(moveDir, vel);
        self->orient[0] = moveDir[1] * -0.899999976f;
        self->orient[1] = 1.0f;
        self->orient[2] = 0.0f;
        self->orient[3] = 0.0f;
        DashRotateOrient(self->orient, self->base.rot[1]);
    } else {
        /* no target: turn toward the input heading and bank */
        result = 5;
        heading = self->input->heading;
        rate = BtlBakuganGetScaledStat50(self);
        turned = BtlBakuganTurnToward(heading, rate, 0.0f, self);
        vel[0] = __builtin_cosf(self->base.rot[1]) * self->dashSpeed;
        vel[2] = __builtin_sinf(self->base.rot[1]) * self->dashSpeed;
        if (self->combat.stats->moveStyle == 0) {
            bank = self->dashBank;
            self->stateFlags |= 1;
            bank = bank + (turned * 0.5f - bank) * 0.100000001f;
            self->dashBank = bank;
            if (!(bank <= 0.600000024f)) {
                bank = 0.600000024f;
            } else if (bank < -0.600000024f) {
                bank = -0.600000024f;
            }
            self->dashBank = bank;
            DashNormalize(bankDir, vel);
            self->orient[0] = bankDir[1] * -0.899999976f;
            self->orient[1] = 1.0f;
            self->orient[2] = self->dashBank;
            self->orient[3] = 0.0f;
            DashRotateOrient(self->orient, self->base.rot[1]);
        }
    }
    if (BtlBakuganIsAirborne(self, 1)) {
        result += 0xd;
    }
    return result;
}
