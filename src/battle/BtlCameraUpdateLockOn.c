// bdc 0x08849504 BtlCameraUpdateLockOn
#include "bdc.h"

/* Camera mode 1 (lock-on): frames the player unit `target` together with its foe
   (`BtlBakuganGetTarget`). Does nothing without a unit; without a foe it falls back to
   `BtlCameraSetDefaultFollow` (no snap) and returns. Otherwise it steps the lock-on blends
   (lockOnHoldBlend while lockOnHoldFrames runs, re-armed to 16 by unit stateFlags 0x4400000;
   lockOnPullBack from stateFlags 0x4000000; lockOnSide, 1 while the HUD is shown), all reset when
   the foe changes. The foe position (100 lower for object kind 0x22, pulled down toward the unit
   beyond 800 units of horizontal distance) gives `near` = 1.2 - dist/1300 capped to 1, which
   eases kindParam3 toward kindParams->param[3] minus the height step. The unit is projected to
   the screen (`GfxCameraProjectPoint`): when it sits low (depth below 0.67) or moves fast,
   lockOnCatchUp rises, otherwise it decays; it sets the 0.35..0.95 ease rate of lookPoint
   (copied to lookTarget first; y eases faster when the unit's head leaves the 30..230 screen
   band) and of focusPoint, which moves toward the unit-to-foe midpoint (copied to
   prevFocusPoint first). The yaw from eye to lookPoint (`atan2f`) is turned to keep the focus
   within an 80-degree outer and a 70-degree (+`g_btlLockOnAngle` while lockOnFlag) inner
   cone, blended with a side-biased yaw by lockOnSide; the up vector eases 20% toward a roll
   from the remaining angle and is renormalised. The eye is rebuilt around lookPoint at
   param[2] distance (plus lockOnSide, unit targetDistance or 220 with the HUD shown, and the
   pull-back), tilted by lockOnElevation eased 20% toward `BtlCameraCalcLockOnPitch`; eye and
   target blend toward it / focusPoint by eyeSnap^2. When the focus is more than 30 above the
   look point and the tilt is above -5 degrees the eye is lifted toward the unit
   (`BtlBakuganIsOnWaterFloor` picks a 15 instead of 10 height factor). Finally
   `BtlCameraApplyCloseUp` runs. */

/* vmin.s against the bank's 1 then vmax.s against the bank's 0: clamps `x` to [0, 1]. A NaN
   follows the vmin/vmax order: +NaN loses vmin (1), -NaN wins vmin and loses vmax (0). */
static inline float BtlLockOnSat(float x)
{
    if (x != x) {
        return __builtin_signbit(x) ? 0.0f : 1.0f;
    }
    return VfSat0(x);
}

/* Raw bits of `f` as a signed word (the binary tests `mfc1` + `bgtz`: positive and non-zero). */
static inline s32 BtlLockOnFloatBits(float f)
{
    union {
        float f;
        s32 i;
    } u;
    u.f = f;
    return u.i;
}

/* Reduces `a` by trunc(a / pi) whole turns, lifts a negative result by one turn, then returns
   -a below pi and 2*pi - a otherwise: the negated shortest angle of `a`. */
static inline float BtlLockOnNegWrapAngle(float a)
{
    a = a - (float)(int)(a * 0.31830987f) * 6.2831855f;
    if (a < 0.0f) {
        a = a + 6.2831855f;
    }
    if (a < 3.1415927f) {
        return -a;
    }
    return 6.2831855f - a;
}

/* v.xyz = saturate[-1, 1](v.xyz / |v.xyz|), scale 0 for a zero length; v.w = 0 (the
   `vscl.t C710` leaves the bank's S713 = 0 in the lane that `sv.q C710` stores to w). */
static inline void BtlLockOnNormalize(float *v)
{
    float lenSq;
    float scale;

    lenSq = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    if (lenSq == 0.0f) {
        scale = 0.0f;
    } else {
        scale = VfRsq(lenSq);
    }
    v[0] = VfSat1(v[0] * scale);
    v[1] = VfSat1(v[1] * scale);
    v[2] = VfSat1(v[2] * scale);
    v[3] = 0.0f;
}

/* v += (to - v) * k on all four lanes. */
static inline void BtlLockOnLerp4(float *v, const float *to, float k)
{
    v[0] = v[0] + (to[0] - v[0]) * k;
    v[1] = v[1] + (to[1] - v[1]) * k;
    v[2] = v[2] + (to[2] - v[2]) * k;
    v[3] = v[3] + (to[3] - v[3]) * k;
}

/* d = s (x) t, quaternions with w last (vqmul.q). */
static inline void BtlLockOnQuatMul(float *d, const float *s, const float *t)
{
    d[0] = s[0] * t[3] + s[1] * t[2] - s[2] * t[1] + s[3] * t[0];
    d[1] = -s[0] * t[2] + s[1] * t[3] + s[2] * t[0] + s[3] * t[1];
    d[2] = s[0] * t[1] - s[1] * t[0] + s[2] * t[3] + s[3] * t[2];
    d[3] = -s[0] * t[0] - s[1] * t[1] - s[2] * t[2] + s[3] * t[3];
}

void BtlCameraUpdateLockOn(BtlCamera *camera)
{
    float tmp[4];
    float rotated[4];
    float unitPos[4];
    float eyeOffset[4];
    float focusOffset[4];
    float mid[4];
    float quat[4];
    float conj[4];
    float offsetQuat[4];
    float half[4];
    float foePos[4];
    float screen[4];
    float rollDir[4];
    float side[4];
    float projPos[4];
    float projHead[4];
    BtlBakugan *unit;
    BtlBakugan *foe;
    u8 hudVisible;
    u8 fast;
    u8 catching;
    u8 follow;
    float dx;
    float dz;
    float dist;
    float scaled;
    float near;
    float dy;
    float drop;
    float rise;
    float speedSq;
    float over;
    float screenLow;
    float step;
    float value;
    float rate;
    float headRate;
    float sideBlend;
    float sideCurve;
    float midBlend;
    float focusRate;
    float spread;
    float eyeAngle;
    float diff;
    float farness;
    float outer;
    float inner;
    float scale;
    float turnRate;
    float bias;
    float excess;
    float roll;
    float yaw;
    float rollYaw;
    float rollCos;
    float rollSin;
    float reach;
    float pull;
    float radius;
    float pitch;
    float halfAngle;
    float halfSin;
    float snap;
    float snapSq;
    float climb;
    float lift;

    if (camera->target == NULL) {
        return;
    }
    if (BtlBakuganGetTarget((BtlBakugan *)camera->target) == NULL) {
        BtlCameraSetDefaultFollow(camera, 0);
        return;
    }
    foe = (BtlBakugan *)BtlBakuganGetTarget((BtlBakugan *)camera->target);

    /* hold blend: towards 1 while the hold runs, towards 0 after; reset on a new foe */
    unit = (BtlBakugan *)camera->target;
    if ((unit->stateFlags & 0x4400000) != 0) {
        camera->lockOnHoldFrames = 16;
    }
    if (camera->lockOnHoldFrames != 0) {
        camera->lockOnHoldFrames = camera->lockOnHoldFrames - 1;
        camera->lockOnFlag = 1;
        if (camera->lockOnFoe == foe) {
            camera->lockOnHoldBlend = camera->lockOnHoldBlend + 0.05f;
        } else {
            camera->lockOnHoldBlend = 1.0f;
        }
    } else {
        camera->lockOnFlag = 0;
        if (camera->lockOnFoe == foe) {
            camera->lockOnHoldBlend = camera->lockOnHoldBlend - 0.05f;
        } else {
            camera->lockOnHoldBlend = 0.0f;
        }
    }
    camera->lockOnHoldBlend = BtlLockOnSat(camera->lockOnHoldBlend);

    if ((((BtlBakugan *)camera->target)->stateFlags & 0x4000000) != 0) {
        camera->lockOnPullBack = camera->lockOnPullBack + 0.1f;
    } else {
        camera->lockOnPullBack = camera->lockOnPullBack - 0.05f;
    }
    camera->lockOnPullBack = BtlLockOnSat(camera->lockOnPullBack);

    /* side bias: 1 while the HUD is shown, else eases by 0.05 toward 0; 0 for a new foe */
    hudVisible = 0;
    if (g_btlHudHidden == 0) {
        hudVisible = 1;
        camera->lockOnSide = 1.0f;
    }
    if (hudVisible != 0) {
        if (camera->lockOnFoe == foe) {
            camera->lockOnSide = camera->lockOnSide + 0.05f;
        } else {
            camera->lockOnSide = 1.0f;
        }
    } else if (camera->lockOnFoe != foe) {
        camera->lockOnSide = 0.0f;
    } else if (!(camera->lockOnSide <= 0.0f)) {
        camera->lockOnSide = camera->lockOnSide - 0.05f;
    } else if (camera->lockOnSide < 0.0f) {
        camera->lockOnSide = camera->lockOnSide + 0.05f;
    }
    camera->lockOnFoe = foe;

    foePos[0] = foe->base.pos[0];
    foePos[1] = foe->base.pos[1];
    foePos[2] = foe->base.pos[2];
    foePos[3] = foe->base.pos[3];
    if (foe->base.base.unk08 == 0x22) {
        foePos[1] = foePos[1] - 100.0f;
    }

    /* dist = horizontal distance unit -> foe (y lane zeroed before the dot product) */
    unit = (BtlBakugan *)camera->target;
    dx = foePos[0] - unit->base.pos[0];
    dz = foePos[2] - unit->base.pos[2];
    dist = __builtin_sqrtf(dx * dx + dz * dz);
    scaled = dist * 0.00076923077f;
    if (!(scaled <= 1.2f)) {
        scaled = 1.2f;
    }
    near = 1.2f - scaled;
    if (!(near <= 1.0f)) {
        near = 1.0f;
    }

    /* a foe high above and far away is pulled down toward the unit's height */
    dy = foePos[1] - ((BtlBakugan *)camera->target)->base.pos[1];
    if (!(dy <= 0.0f) && !(dist <= 800.0f)) {
        drop = (dist - 800.0f) * 0.0008f;
        if (!(drop <= 1.0f)) {
            drop = 1.0f;
        }
        foePos[1] = foePos[1] - drop * 0.8f * dy;
    }

    rise = (foePos[1] - ((BtlBakugan *)camera->target)->base.pos[1]) * 0.1f;
    if (rise < -100.0f) {
        rise = -100.0f;
    } else if (camera->kindParams->param[3] < rise) {
        rise = camera->kindParams->param[3];
    }
    if (rise < 0.0f) {
        rise = rise * 0.5f;
    }
    rise = near * rise;
    camera->kindParam3 = camera->kindParam3 +
                         ((camera->kindParams->param[3] - rise) - camera->kindParam3) * 0.1f;

    /* unitPos = unit position raised by kindParam3; speedSq = horizontal speed squared */
    unit = (BtlBakugan *)camera->target;
    unitPos[0] = unit->base.pos[0];
    unitPos[1] = unit->base.pos[1];
    unitPos[2] = unit->base.pos[2];
    unitPos[3] = unit->base.pos[3];
    unitPos[1] = unitPos[1] + camera->kindParam3;
    unit = (BtlBakugan *)camera->target;
    speedSq = unit->base.velocity[0] * unit->base.velocity[0] +
              unit->base.velocity[2] * unit->base.velocity[2];
    fast = 0;
    if (!(speedSq <= 841.0f)) {
        fast = 1;
    }

    catching = 0;
    screenLow = 0.0f;
    unit = (BtlBakugan *)camera->target;
    projPos[0] = unit->base.pos[0];
    projPos[1] = unit->base.pos[1];
    projPos[2] = unit->base.pos[2];
    projPos[3] = unit->base.pos[3];
    projPos[1] = projPos[1] + camera->kindParams->param[3];
    GfxCameraProjectPoint(&camera->base, screen, projPos);

    /* over = how far the unit's depth is below 0.67, times 10 */
    over = 0.67f - screen[2];
    if (BtlLockOnFloatBits(over) > 0) {
        over = over * 10.0f;
    } else {
        over = 0.0f;
    }
    if (!(over <= 0.0f)) {
        screenLow = (over - 0.4f) * 10.0f;
        if (!(BtlLockOnFloatBits(screenLow) > 0)) {
            screenLow = 0.0f;
        }
        step = over * 0.08f;
        if (!(step <= 0.2f)) {
            step = 0.2f;
        }
        if (fast != 0) {
            step = step * 3.0f;
        }
        camera->lockOnMidBlend = camera->lockOnMidBlend - step;
        catching = 1;
        value = camera->lockOnCatchUp + over * 0.1f;
        if (!(value <= 1.0f)) {
            value = 1.0f;
        }
        camera->lockOnCatchUp = value;
    }

    if (catching == 0) {
        camera->lockOnRelax = BtlLockOnSat(camera->lockOnRelax + 0.033333335f);
    } else {
        camera->lockOnRelax = camera->lockOnRelax * 0.92f;
    }
    camera->lockOnMidBlend =
        camera->lockOnMidBlend +
        (1.0f - __builtin_cosf(camera->lockOnRelax * 3.1415927f)) * 0.5f * 0.2f;
    camera->lockOnMidBlend = BtlLockOnSat(camera->lockOnMidBlend);

    if (fast != 0) {
        value = camera->lockOnCatchUp + 0.1f;
        if (!(value <= 1.0f)) {
            value = 1.0f;
        }
        camera->lockOnCatchUp = value;
    } else if (catching == 0) {
        camera->lockOnCatchUp = camera->lockOnCatchUp * 0.9f;
    }
    rate = camera->lockOnCatchUp * 0.6f + 0.35f;

    /* the look point follows the unit unless it stands still in a no-turn state */
    unit = (BtlBakugan *)camera->target;
    follow = 1;
    if (unit->noTurn != 0 && (((BtlBakugan *)camera->target)->stateFlags & 0x40000) == 0 &&
        (foe->stateFlags & 0x40000) == 0 && rise <= 15.0f &&
        ((BtlBakugan *)camera->target)->state != 0xb && foe->state != 0xb) {
        follow = 0;
    }

    if (follow != 0) {
        camera->lookTarget[0] = camera->lookPoint[0];
        camera->lookTarget[1] = camera->lookPoint[1];
        camera->lookTarget[2] = camera->lookPoint[2];
        camera->lookTarget[3] = camera->lookPoint[3];
        camera->lookPoint[0] = camera->lookPoint[0] + (unitPos[0] - camera->lookPoint[0]) * rate;
        camera->lookPoint[2] = camera->lookPoint[2] + (unitPos[2] - camera->lookPoint[2]) * rate;
        headRate = rate * 0.6f;
        if (!(screen[1] <= 230.0f)) {
            value = rate * ((screen[1] - 230.0f) * 0.008f + 0.6f);
            if (value <= 1.0f) {
                headRate = value;
            } else {
                headRate = 1.0f;
            }
        } else {
            unit = (BtlBakugan *)camera->target;
            projHead[0] = unit->base.pos[0];
            projHead[1] = unit->base.pos[1];
            projHead[2] = unit->base.pos[2];
            projHead[3] = unit->base.pos[3];
            projHead[1] = projHead[1] + ((BtlBakugan *)camera->target)->height;
            GfxCameraProjectPoint(&camera->base, screen, projHead);
            if (screen[1] < 30.0f) {
                value = rate * ((30.0f - screen[1]) * 0.008f + 0.6f);
                if (value <= 1.0f) {
                    headRate = value;
                } else {
                    headRate = 1.0f;
                }
            }
        }
        camera->lookPoint[1] = camera->lookPoint[1] +
                               (unitPos[1] - camera->lookPoint[1]) * headRate;
    }

    sideBlend = camera->lockOnSide;
    if (!(sideBlend <= 1.0f)) {
        sideBlend = 1.0f;
    } else if (sideBlend < -1.0f) {
        sideBlend = -1.0f;
    }
    camera->lockOnSide = sideBlend;
    sideCurve = (1.0f - __builtin_cosf(sideBlend * sideBlend * 3.1415927f)) * 0.5f;
    if (sideBlend < 0.0f) {
        sideCurve = -sideCurve;
    }
    midBlend =
        (1.0f - __builtin_cosf(camera->lockOnMidBlend * 3.1415927f)) * 0.5f * 0.2f + 0.05f;
    midBlend = midBlend + sideCurve * 0.25f;
    if ((((BtlBakugan *)camera->target)->flags & 8) != 0) {
        midBlend = 1.0f;
    }

    /* mid = unit position + (foePos - unit position) * midBlend, raised by kindParam3;
       prevFocusPoint = focusPoint */
    unit = (BtlBakugan *)camera->target;
    mid[0] = unit->base.pos[0];
    mid[1] = unit->base.pos[1];
    mid[2] = unit->base.pos[2];
    mid[3] = unit->base.pos[3];
    BtlLockOnLerp4(mid, foePos, midBlend);
    mid[1] = mid[1] + camera->kindParam3;
    camera->prevFocusPoint[0] = camera->focusPoint[0];
    camera->prevFocusPoint[1] = camera->focusPoint[1];
    camera->prevFocusPoint[2] = camera->focusPoint[2];
    camera->prevFocusPoint[3] = camera->focusPoint[3];
    if (follow != 0 && ((BtlBakugan *)camera->target)->noTurn == 0) {
        focusRate = rate;
    } else {
        focusRate = rate * ((BtlBakugan *)camera->target)->targetCloseness * 0.1f;
    }
    /* focusPoint eases toward mid; focusOffset.xyz = focusPoint - lookPoint (w = focusPoint.w) */
    BtlLockOnLerp4(camera->focusPoint, mid, focusRate);
    focusOffset[0] = camera->focusPoint[0] - camera->lookPoint[0];
    focusOffset[1] = camera->focusPoint[1] - camera->lookPoint[1];
    focusOffset[2] = camera->focusPoint[2] - camera->lookPoint[2];
    focusOffset[3] = camera->focusPoint[3];

    spread = 1.2217305f;
    if (camera->lockOnFlag != 0) {
        spread = spread + g_btlLockOnAngle;
    }
    /* eyeOffset.xyz = lookPoint - eye (w = lookPoint.w) */
    eyeOffset[0] = camera->lookPoint[0] - camera->base.eye[0];
    eyeOffset[1] = camera->lookPoint[1] - camera->base.eye[1];
    eyeOffset[2] = camera->lookPoint[2] - camera->base.eye[2];
    eyeOffset[3] = camera->lookPoint[3];
    eyeAngle = atan2f(eyeOffset[2], eyeOffset[0]);
    diff = BtlLockOnNegWrapAngle(eyeAngle - atan2f(focusOffset[2], focusOffset[0]));

    /* turn limits: 80 degrees outer, `spread` inner, narrowed as the foe gets farther and
       while the hold blend is up */
    farness = 1.0f - near;
    outer = 1.3962634f - 1.3962634f * farness * 0.7f;
    inner = spread - spread * farness * 0.7f;
    scale =
        1.0f - (1.0f - __builtin_cosf(camera->lockOnHoldBlend * 3.1415927f)) * 0.5f * 0.3f;
    outer = outer * scale;
    inner = inner * scale;
    turnRate = BtlLockOnSat(rate + scale * 0.5f);

    if (diff < 0.0f) {
        bias = sideCurve * (eyeAngle + (diff + 1.8151424f) * turnRate * 0.5f);
        if (diff < -outer) {
            eyeAngle = eyeAngle + (diff + outer) * turnRate * BtlLockOnSat(outer - diff);
        } else if (!(diff <= -inner) && hudVisible == 0 && camera->lockOnFlag != 0) {
            excess = BtlLockOnSat(diff + inner) * 3.0f;
            value = (1.0f - __builtin_cosf(camera->lockOnHoldBlend * 3.1415927f)) * 0.5f;
            eyeAngle = eyeAngle - (-diff - inner) * turnRate * excess * value;
        }
    } else {
        bias = sideCurve * (eyeAngle + (diff - 1.8151424f) * turnRate * 0.5f);
        if (!(diff <= outer)) {
            excess = diff - outer;
            eyeAngle = eyeAngle + excess * turnRate * BtlLockOnSat(excess);
        } else if (diff < inner && hudVisible == 0 && camera->lockOnFlag != 0) {
            excess = inner - diff;
            value = BtlLockOnSat(excess) * 3.0f;
            eyeAngle = eyeAngle - excess * turnRate * value *
                                  ((1.0f - __builtin_cosf(camera->lockOnHoldBlend * 3.1415927f)) *
                                   0.5f);
        }
    }
    yaw = eyeAngle * (1.0f - sideCurve) + bias;

    roll = diff * 0.2f;
    if (!(roll <= 0.04f)) {
        roll = roll - 0.04f;
    } else if (roll < -0.04f) {
        roll = roll + 0.04f;
    } else {
        roll = 0.0f;
    }
    roll = roll * (1.0f - sideCurve);
    rollDir[0] = __builtin_sinf(roll);
    rollDir[1] = __builtin_cosf(roll);
    rollDir[2] = 0.0f;
    rollDir[3] = 0.0f;

    /* rollDir rotated about y by yaw + pi/2 (vrot [C,0,-S,0] / [S,0,C,0] dotted with it) */
    rollYaw = yaw + 1.5707964f;
    rollCos = __builtin_cosf(rollYaw);
    rollSin = __builtin_sinf(rollYaw);
    tmp[0] = rollDir[0] * rollCos + rollDir[1] * 0.0f + rollDir[2] * -rollSin;
    tmp[2] = rollDir[0] * rollSin + rollDir[1] * 0.0f + rollDir[2] * rollCos;
    rollDir[0] = tmp[0];
    rollDir[2] = tmp[2];

    /* up eases 20% toward rollDir, then up.xyz is normalised and saturated to [-1, 1], up.w = 0 */
    BtlLockOnLerp4(camera->base.up, rollDir, 0.2f);
    BtlLockOnNormalize(camera->base.up);

    if (!(yaw <= 3.1415927f)) {
        yaw = yaw - 6.2831855f;
    } else if (yaw <= -3.1415927f) {
        yaw = yaw + 6.2831855f;
    }

    reach = ((BtlBakugan *)camera->target)->targetDistance;
    if (hudVisible != 0) {
        reach = 220.0f;
    }
    pull = camera->lockOnPullBack - 1.0f;
    pull = (1.0f - __builtin_cosf((1.0f - pull * pull) * 3.1415927f)) * 0.5f * 60.0f + screenLow;
    camera->yaw = yaw;
    radius = -camera->kindParams->param[2] - sideCurve * (reach - 180.0f) * 1.3f - pull;

    /* eyeOffset = (cos yaw, 0, sin yaw) * radius, w = 0 (vrot [C,0,S,0] + vscl.t) */
    eyeOffset[0] = __builtin_cosf(yaw) * radius;
    eyeOffset[1] = 0.0f * radius;
    eyeOffset[2] = __builtin_sinf(yaw) * radius;
    eyeOffset[3] = 0.0f;

    pitch = BtlCameraCalcLockOnPitch(camera->focusPoint[1], camera->lookPoint[1],
                                     camera->lockOnPitch, near);
    camera->lockOnElevation = camera->lockOnElevation +
                              (pitch - camera->lockOnElevation) * 0.2f;

    /* side = normalised (eyeOffset.z, eyeOffset.y, -eyeOffset.x) saturated to [-1, 1];
       quat = (side * sin(t / 2), cos(t / 2)) for t = -lockOnElevation (vcst 1/PI quarter turns) */
    side[0] = eyeOffset[2];
    side[1] = eyeOffset[1];
    side[2] = -eyeOffset[0];
    side[3] = eyeOffset[3];
    BtlLockOnNormalize(side);
    halfAngle = 0.318309873f * -camera->lockOnElevation;
    quat[3] = VfCosQuarter(halfAngle);
    halfSin = VfSinQuarter(halfAngle);
    quat[0] = side[0] * halfSin;
    quat[1] = side[1] * halfSin;
    quat[2] = side[2] * halfSin;

    snap = camera->eyeSnap + 0.1f;
    if (!(snap <= 1.0f)) {
        snap = 1.0f;
    }
    camera->eyeSnap = snap;

    /* rotated = quat * (eyeOffset.xyz, 0) * conj(quat); tmp.xyz = lookPoint.xyz + rotated.xyz,
       tmp.w = lookPoint.w; eye += (tmp - eye) * eyeSnap^2;
       target += (focusPoint - target) * eyeSnap^2 */
    conj[0] = -quat[0];
    conj[1] = -quat[1];
    conj[2] = -quat[2];
    conj[3] = quat[3];
    offsetQuat[0] = eyeOffset[0];
    offsetQuat[1] = eyeOffset[1];
    offsetQuat[2] = eyeOffset[2];
    offsetQuat[3] = 0.0f;
    BtlLockOnQuatMul(half, quat, offsetQuat);
    BtlLockOnQuatMul(rotated, half, conj);
    tmp[0] = camera->lookPoint[0] + rotated[0];
    tmp[1] = camera->lookPoint[1] + rotated[1];
    tmp[2] = camera->lookPoint[2] + rotated[2];
    tmp[3] = camera->lookPoint[3];
    snapSq = camera->eyeSnap * camera->eyeSnap;
    BtlLockOnLerp4(camera->base.eye, tmp, snapSq);
    snapSq = camera->eyeSnap * camera->eyeSnap;
    BtlLockOnLerp4(camera->base.target, camera->focusPoint, snapSq);

    /* focus well above the look point and tilt above -5 degrees: lift the eye toward the unit */
    if (!(camera->focusPoint[1] <= camera->lookPoint[1] + 30.0f)) {
        climb = (-camera->lockOnElevation - -0.08726646f) * near;
        if (!(climb <= 0.0f)) {
            lift = 10.0f;
            if (BtlBakuganIsOnWaterFloor((BtlBakugan *)camera->target) != 0) {
                lift = 15.0f;
            }
            unitPos[1] = unitPos[1] + lift * climb * camera->kindParam3;
            BtlLockOnLerp4(camera->base.eye, unitPos, climb);
            camera->base.target[1] = camera->base.target[1] + climb * 140.0f;
        }
    }
    BtlCameraApplyCloseUp(camera);
}
