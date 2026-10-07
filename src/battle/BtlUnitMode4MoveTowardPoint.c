// bdc 0x0885d038 BtlUnitMode4MoveTowardPoint
#include "bdc.h"

/* Movement step of the mode-4 unit's retreat (`BtlUnitMode4UpdateRetreat`): runs toward the
   point `target` (16-byte aligned, read with `lv.q`). Counts the dash in statistic 0xe
   (`BtlStatsAddCounter`) when the unit has battle stats, sets state flag 0x1000000, lowers
   `dashSpeed` by stat54 x the motion time scale (`BtlBakuganGetStat54`,
   `GfxGetMotionTimeScale`) but not below stat4C (`BtlBakuganGetScaledStat4C`), and sets the
   vertical speed by the stat table's `moveStyle`: 0 = gravity + 0.3 x the height missing to
   `groundY + cruiseHeight` + `dashLift`; 1 = hover toward that height (or fall, clamped to <= 0, when
   `cruiseHeight` is 0); other values leave it. `dashLift` grows by 0.7 x stat64 (capped at stat6C -
   gravity) while the unit is below the target, else decays by 0.8. `dashHeading` turns toward
   the target's atan2 by stat50 x 0.8 x the wrapped angle difference and is wrapped to -pi..pi;
   `BtlBakuganClassifyTargetDirection` picks the run motion (2 -> 7, 3 -> 6, else 5). Unless
   `dashMode` is 1, the vertical speed also gains cos(|difference|) x the normalised target
   direction's y x `dashSpeed` x 0.6 x (1 + its dot with the velocity direction), clamped to
   +-0.7 x `jumpSpeed`. Then it turns the model (`BtlBakuganTurnToward` 0.3), sets the
   horizontal velocity to `dashSpeed` along `dashHeading`, sets state flag 1, builds `orient`
   from the velocity direction (x = -0.9 x its y, y = 1) rotated by the heading `rot[1]` and
   normalised, and plays the looping motion (`BtlBakuganPlayMotion` blend 0.2), +13 when
   airborne (`BtlBakuganIsAirborne`). VFPU bank constants S703 (2/pi, so `vcos`/`vrot` take
   radians) and S713 (0: zero-length normalise scale and the w lane of normalised vectors) are
   literals. Variant of `BtlBakuganDashStep`. */
/* vdot/vrsq normalise of xyz: a zero vector scales by 0 (S713), each lane saturated to [-1, 1]
   (vpfxd), lane 3 is C710's w, the bank's S713 (0) */
static void Mode4Normalize(float *out, const float *v)
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

void BtlUnitMode4MoveTowardPoint(BtlUnitMode4 *self, const float *target)
{
    BtlBakugan *unit = &self->base;
    float velDir[4];
    float toTarget[4];
    float velDir2[4];
    float rotated[4];
    float speed;
    float floor;
    float v;
    float cap;
    float lift;
    float heading;
    float diff;
    float shape;
    float dot;
    float cosTurn;
    float c;
    float s;
    s8 style;
    s32 dir;
    s32 motion;

    motion = 5;
    if (unit->stats != NULL) {
        BtlStatsAddCounter(unit->stats, 0xe, 1);
    }
    unit->stateFlags = unit->stateFlags | 0x1000000;

    /* dashSpeed decays by stat54 per frame, down to stat4C */
    speed = unit->dashSpeed;
    v = BtlBakuganGetStat54(unit);
    v = v * GfxGetMotionTimeScale();
    speed = speed - v;
    floor = BtlBakuganGetScaledStat4C(unit);
    if (!(floor <= speed)) {
        speed = floor;
    }
    unit->dashSpeed = speed;

    /* vertical speed by movement style */
    style = unit->combat.stats->moveStyle;
    if (style == 0) {
        v = (unit->groundY + unit->combat.stats->cruiseHeight) - unit->base.pos[1];
        if (v < 0.0f) {
            v = 0.0f;
        }
        unit->base.velocity[1] = unit->gravity + v * 0.3f + unit->dashLift;
    } else if (style == 1) {
        if (unit->combat.stats->cruiseHeight == 0.0f) {
            v = unit->gravity * 0.7f + unit->dashLift * 0.2f + unit->base.velocity[1];
            unit->base.velocity[1] = v;
            if (!(v <= 0.0f)) {
                v = 0.0f;
            }
            unit->base.velocity[1] = v;
        } else {
            v = (unit->groundY + unit->combat.stats->cruiseHeight) - unit->base.pos[1];
            if (v < 0.0f) {
                unit->base.velocity[1] =
                    unit->gravity * 0.5f + unit->dashLift * 0.2f + unit->base.velocity[1];
            } else {
                v = v * 0.2f;
                if (!(v <= 2.0f)) {
                    v = 2.0f;
                }
                unit->base.velocity[1] = unit->gravity * 0.5f + v + unit->dashLift * 0.2f +
                                         unit->base.velocity[1];
            }
            unit->base.velocity[1] =
                unit->base.velocity[1] +
                (unit->gravity * 0.7f - unit->base.velocity[1]) * 0.1f;
        }
    }

    /* dashLift: climb while below the target, else decay */
    if (unit->base.pos[1] < target[1]) {
        lift = unit->dashLift;
        lift = lift + BtlBakuganGetScaledStat64(unit) * 0.7f;
        cap = BtlBakuganGetScaledStat6C(unit) - unit->gravity;
        if (cap < lift) {
            lift = cap;
        }
        unit->dashLift = lift;
    } else {
        unit->dashLift = unit->dashLift * 0.8f;
    }

    /* signed angle from the target bearing to dashHeading */
    heading = unit->dashHeading;
    diff = heading - atan2f(target[2] - unit->base.pos[2], target[0] - unit->base.pos[0]);
    diff = diff - (float)(s32)(diff * 0.318309873f) * 6.28318548f; /* 0x3ea2f983, 0x40c90fdb */
    if (diff < 0.0f) {
        diff = diff + 6.28318548f;
    }
    if (diff < 3.14159274f) {
        diff = -diff;
    } else {
        diff = 6.28318548f - diff;
    }
    heading = unit->dashHeading;
    unit->dashHeading = heading + diff * BtlBakuganGetScaledStat50(unit) * 0.8f;
    if (!(unit->dashHeading <= 3.14159274f)) {
        unit->dashHeading = unit->dashHeading - 6.28318548f;
    } else if (unit->dashHeading <= -3.14159274f) {
        unit->dashHeading = unit->dashHeading + 6.28318548f;
    }
    dir = BtlBakuganClassifyTargetDirection(unit->dashHeading, unit);

    if (unit->combat.stats->dashMode != 1) {
        /* toTarget = normalize(target - pos) (w = target[3] before the normalise),
           velDir = normalize(velocity) */
        toTarget[0] = target[0] - unit->base.pos[0];
        toTarget[1] = target[1] - unit->base.pos[1];
        toTarget[2] = target[2] - unit->base.pos[2];
        toTarget[3] = target[3];
        Mode4Normalize(toTarget, toTarget);
        Mode4Normalize(velDir, unit->base.velocity);
        dot = velDir[0] * toTarget[0] + velDir[1] * toTarget[1] + velDir[2] * toTarget[2];
        /* vcos of |diff| x S703: cos of the angle in radians */
        cosTurn = __builtin_cosf(__builtin_fabsf(diff));
        shape = (dot + 1.0f) * 0.6f;
        v = unit->base.velocity[1] + cosTurn * toTarget[1] * unit->dashSpeed * shape;
        unit->base.velocity[1] = v;
        cap = unit->combat.stats->jumpSpeed * 0.7f;
        if (!(v <= cap)) {
            v = cap;
        } else if (v < -cap) {
            v = -cap;
        }
        unit->base.velocity[1] = v;
    }

    switch (dir) {
    case 2:
        motion = 7;
        break;
    case 3:
        motion = 6;
        break;
    default:
        motion = 5;
        break;
    }

    BtlBakuganTurnToward(unit->dashHeading, 0.3f, 0.0f, unit);

    /* velocity.xz = (cos, sin)(dashHeading) * dashSpeed (vrot [C,0,S,0], lanes 0 and 2 stored) */
    heading = unit->dashHeading;
    speed = unit->dashSpeed;
    unit->base.velocity[0] = __builtin_cosf(heading) * speed;
    unit->base.velocity[2] = __builtin_sinf(heading) * speed;
    unit->stateFlags = unit->stateFlags | 1;
    Mode4Normalize(velDir2, unit->base.velocity);
    unit->orient[0] = velDir2[1] * -0.899999976f; /* 0xbf666666 */
    unit->orient[1] = 1.0f;
    unit->orient[2] = 0.0f;
    unit->orient[3] = 0.0f;
    /* orient.xz rotated by rot[1] (vrot [C,0,-S,0] / [S,0,C,0] + vdot.t), then normalised */
    c = __builtin_cosf(unit->base.rot[1]);
    s = __builtin_sinf(unit->base.rot[1]);
    rotated[0] = unit->orient[0] * c + unit->orient[1] * 0.0f + unit->orient[2] * -s;
    rotated[1] = unit->orient[1];
    rotated[2] = unit->orient[0] * s + unit->orient[1] * 0.0f + unit->orient[2] * c;
    rotated[3] = unit->orient[3];
    Mode4Normalize(unit->orient, rotated);

    if (BtlBakuganIsAirborne(unit, 1) != 0) {
        motion = motion + 0xd;
    }
    BtlBakuganPlayMotion(0.2f, unit, motion, 1, 0);
}
