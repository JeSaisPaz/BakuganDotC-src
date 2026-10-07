// bdc 0x08894514 BtlAiExecDetourMove
#include "bdc.h"

/* |a - b| over x, y, z (vsub.q, vdot.t, vsqrt.s). */
static inline float BtlAiDetourDist(const float *a, const float *b)
{
    float dx = a[0] - b[0];
    float dy = a[1] - b[1];
    float dz = a[2] - b[2];

    return __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
}

/* In-place normalise of `v` (x, y, z), each component saturated to [-1, 1]; a zero vector takes the
   bank zero S713 as its inverse length. Lane w is masked in the vscl.t, so the sv.q of C710 stores
   S713 (bank 0.0f) there. */
static inline void BtlAiDetourNormalize(float *v)
{
    float lenSq = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    float inv = VfRsq(lenSq);

    if (lenSq == 0.0f) {
        inv = 0.0f;
    }
    v[0] = VfSat1(v[0] * inv);
    v[1] = VfSat1(v[1] * inv);
    v[2] = VfSat1(v[2] * inv);
    v[3] = 0.0f;
}

/* Executes the detour command record `cmd` (layout as in `BtlAiExecObstacleDash`) of
   `BtlAi`: advances the command timer by 1/30 s (clamped to its limit, setting
   the expired byte) and sets side flag 0x4000 of `ai+0x9d4`. State 0 clears the pad actions and
   casts a forward ray of length `ai+0xa18` (`BtlAiRaycastDir8`); a hit of kind 3 in
   `BtlAiIsScoreMode1` mode marks it failed (state 4), but if the owner is free or idle
   (state 0) on that same call it moves to state 1 anyway, otherwise it returns. State 1 picks the
   detour point `ai+0x9c0` — on stages 36/38 (`GameStageIs36Or38`) with a saved target
   `ai+0x970` more than 2000 away, the nearest of the three waypoints of `g_btlAiRangeConsts`
   that is over 500 away (waypoint 0 if none is), otherwise `owner + range × (cos, 0, sin)(angle)`
   from `BtlAiPickOpenHeading` (range starts at 1000, angle wrapped to (-pi, pi]) — and records
   the planned distance (truncated) and the odometer `ai+0x9a0`, clears `ai+0xa20` and moves to
   state 2. State 2 turns the heading to the point (`atan2f`) into a direction
   (`BtlAiAngleToDir8`); when the owner's XZ velocity direction faces the point (dot not below
   0.85) a blocked ray of length `ai+0xa18` (`BtlAiRaycastBlocked`) marks it failed (state 4)
   and returns; within 30 units of the point it sets state 4 and returns; otherwise it continues
   as state 3. State 3 steers (`BtlAiPadSteer`, no hold; direction 0 when entered directly)
   while the odometer has advanced less than the planned distance, counting frames in
   `ai+0xa20`; after 1 s it ends (state 4) when the front is blocked over the owner's body radius
   (`BtlAiIsFrontBlocked`), else returns. Finishing (state 4, or state 3 once the distance is
   covered) sets the finished byte and, on stages 36/38, `ai+0xa26` = 30, which makes command 9
   of `BtlAiRunMoveRules` dash straight ahead. Unknown states return at once. */
void BtlAiExecDetourMove(BtlAi *self, BtlAiCommand *cmd)
{
    float angle;
    float range;
    float toPoint[4];
    float facing[4];
    float from[4];
    float ray[4];
    const float *pos;
    const float *wp;
    u16 dir = 0;
    s32 hit;
    s32 busy;
    s32 free;
    s32 picked;
    s8 nearest;
    u8 i;
    float dist;
    float best;
    float dot;
    float lenSq;
    float k;
    float frames;
    float radius;

    angle = 0.0f;
    range = 1000.0f;
    if (cmd->timerExpired == 0) {
        cmd->timerElapsed = cmd->timerElapsed + 0.033333335f;
        if (!(cmd->timerElapsed < cmd->timerLimit)) {
            cmd->timerElapsed = cmd->timerLimit;
            cmd->timerExpired = 1;
        }
    }
    self->moveFlags = self->moveFlags | 0x4000;

    switch (cmd->state) {
    case 0:
        hit = BtlAiRaycastDir8(self->rayLength, self, 0);
        if (hit != 0 && BtlAiIsScoreMode1() != 0 && hit == 3) {
            cmd->failed = 1;
            cmd->state = 4;
        }
        self->pad.cur.aiActions = 0;
        self->pad.prev.aiActions = 0;
        /* wait until the owner is free to move (or idle) */
        free = 0;
        if ((self->owner->stateFlags & 0x400000) == 0) {
            busy = 0;
            if (self->owner->state == 8 || self->owner->state == 10) {
                busy = 1;
            }
            if (!busy && (self->owner->commands & 0xc00) == 0 &&
                (self->owner->stateFlags & 0x100) == 0) {
                free = 1;
            }
        }
        if (!free && self->owner->state != 0) {
            return;
        }
        cmd->state = 1;
        /* fallthrough */
    case 1:
        picked = 0;
        if (GameStageIs36Or38() != 0 && self->savedTarget != NULL &&
            !(BtlAiDetourDist(self->owner->base.pos, self->savedTarget->base.pos) <= 2000.0f)) {
            best = g_btlAiFloatInf;
            nearest = 0;
            for (i = 0; i < 3; i++) {
                dist = BtlAiDetourDist(self->owner->base.pos,
                                       g_btlAiRangeConsts.detourWaypoints[i]);
                if (dist <= 500.0f || best <= dist) {
                    continue;
                }
                nearest = (s8)i;
                best = dist;
            }
            wp = g_btlAiRangeConsts.detourWaypoints[nearest];
            self->detourPoint[0] = wp[0];
            self->detourPoint[1] = wp[1];
            self->detourPoint[2] = wp[2];
            self->detourPoint[3] = wp[3];
            range = BtlAiDetourDist(self->owner->base.pos, self->detourPoint);
            picked = 1;
        }
        if (picked == 0) {
            BtlAiPickOpenHeading(self, &angle, &range);
            if (!(angle <= 3.1415927f)) {
                angle = angle - 6.2831855f;
            } else if (angle <= -3.1415927f) {
                angle = angle + 6.2831855f;
            }
            /* The binary first stores the bank zero C720 to the point, then overwrites all four
               lanes: vrot.q [C,0,S,0] of angle × S703 (2/π), scaled by range over x, y, z (w stays
               0), plus the owner position over x, y, z. */
            pos = self->owner->base.pos;
            self->detourPoint[0] = __builtin_cosf(angle) * range + pos[0];
            self->detourPoint[1] = 0.0f * range + pos[1];
            self->detourPoint[2] = __builtin_sinf(angle) * range + pos[2];
            self->detourPoint[3] = 0.0f;
        }
        cmd->arg = (s32)range;
        cmd->argF = self->odometer;
        self->detourFrames = 0.0f;
        cmd->state = 2;
        return;
    case 2:
        angle = atan2f(self->detourPoint[2] - self->owner->base.pos[2],
                       self->detourPoint[0] - self->owner->base.pos[0]);
        if (!(angle <= 3.1415927f)) {
            angle = angle - 6.2831855f;
        } else if (angle <= -3.1415927f) {
            angle = angle + 6.2831855f;
        }
        dir = BtlAiAngleToDir8(angle);
        /* toPoint = unit XZ vector from the owner to the detour point */
        pos = self->owner->base.pos;
        toPoint[0] = self->detourPoint[0] - pos[0];
        toPoint[1] = 0.0f;
        toPoint[2] = self->detourPoint[2] - pos[2];
        toPoint[3] = self->detourPoint[3];
        BtlAiDetourNormalize(toPoint);
        /* facing = unit XZ vector of the owner's velocity */
        facing[0] = self->owner->base.velocity[0];
        facing[1] = 0.0f;
        facing[2] = self->owner->base.velocity[2];
        facing[3] = self->owner->base.velocity[3];
        BtlAiDetourNormalize(facing);
        dot = toPoint[0] * facing[0] + toPoint[1] * facing[1] + toPoint[2] * facing[2];
        if (!(dot < 0.85f)) {
            /* facing = facing / |facing| × rayLength (S713 again the zero-length fallback; lane w
               keeps S713 = 0) */
            lenSq = facing[0] * facing[0] + facing[1] * facing[1] + facing[2] * facing[2];
            k = VfRsq(lenSq);
            if (lenSq == 0.0f) {
                k = 0.0f;
            }
            k = k * self->rayLength;
            facing[0] = facing[0] * k;
            facing[1] = facing[1] * k;
            facing[2] = facing[2] * k;
            facing[3] = 0.0f;
            pos = self->owner->base.pos;
            from[0] = pos[0];
            from[1] = pos[1];
            from[2] = pos[2];
            from[3] = pos[3];
            ray[0] = facing[0];
            ray[1] = facing[1];
            ray[2] = facing[2];
            ray[3] = facing[3];
            if (BtlAiRaycastBlocked(self, from, ray, NULL) != 0) {
                cmd->failed = 1;
                cmd->state = 4;
                return;
            }
        }
        if (BtlAiDetourDist(self->owner->base.pos, self->detourPoint) <= 30.0000019f) {
            cmd->state = 4;
            return;
        }
        /* fallthrough */
    case 3:
        if (self->odometer - cmd->argF < (float)cmd->arg) {
            BtlAiPadSteer(self, (s16)dir, 0);
            frames = self->detourFrames + 1.0f;
            radius = self->owner->combat.stats->bodyRadius;
            self->detourFrames = frames;
            if (frames * 0.033333335f < 1.0f) {
                return;
            }
            if (BtlAiIsFrontBlocked(radius, self) == 0) {
                return;
            }
            cmd->state = 4;
        }
        /* fallthrough */
    case 4:
        if (GameStageIs36Or38() != 0) {
            self->forwardDashFrames = 30;
        }
        cmd->finished = 1;
        break;
    default:
        break;
    }
}
