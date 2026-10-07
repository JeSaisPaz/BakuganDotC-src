// bdc 0x0882dea4 BtlHudAnimateFinishBannerPair
#include "bdc.h"

/* Two-sprite end-of-battle banner (modes 1/2, `BtlHudStartFinishMode1` /
   `BtlHudStartFinishMode2`) of the battle HUD, driven by finishState like
   `BtlHudAnimateFinishBanner`. `top` is HUD sprite 0x86 (shown only when profile word 7 is set,
   `SaveProfileGetWord`) moving to finishTarget, `bottom` is sprite 0x85 moving to finishTarget2.
   1: resets finishHold/finishAngle, places both 300 px right of their targets with alpha 0, then
   runs step 2. 2: finishAngle grows 2.5 degrees per frame, clamped to [0, pi/2]; alpha =
   sin(angle) (VFPU vsin of angle * 2/pi in quarter turns) and both positions move that fraction of
   the way to their targets; once the top alpha >= 0.95 both snap (alpha 1), a 60-frame hold is
   armed and step 3 runs. 3: counts finishHold down; once <= 0 the angle is reset to pi/2 and step 10
   runs. 10: the angle shrinks 6 degrees per frame (same clamp), alpha = angle (no sine) and both
   positions move (1 - angle) of the way to their targets shifted 300 px left; once the top alpha <=
   0.05 both are hidden (alpha 0, flags bit 0 cleared) and the state returns to 0. States 4..9 and
   out-of-range ones reset to 0; 0 does nothing. Quad copies are `lv.q`/`sv.q`, the lerps
   `vsub.q`/`vscl.q`/`vadd.q`. */

#define BANNER_HALF_PI 1.57079637f

/* 16-byte quad copy (lv.q/sv.q through C000). */
static inline void BtlHudFinishPairCopyQ(float *dst, const float *src)
{
    float x = src[0];
    float y = src[1];
    float z = src[2];
    float w = src[3];

    dst[0] = x;
    dst[1] = y;
    dst[2] = z;
    dst[3] = w;
}

/* Lerp `pos += (to - pos) * t` on four floats (vsub.q/vscl.q/vadd.q). */
static inline void BtlHudFinishPairLerp(float *pos, const float *to, float t)
{
    int i;

    for (i = 0; i < 4; i++) {
        pos[i] = pos[i] + (to[i] - pos[i]) * t;
    }
}

static inline float BtlHudFinishPairClamp(float angle)
{
    if (angle < 0.0f) {
        return 0.0f;
    }
    if (!(angle <= BANNER_HALF_PI)) {
        return BANNER_HALF_PI;
    }
    return angle;
}

void BtlHudAnimateFinishBannerPair(BtlHud *self)
{
    GfxSprite *top = self->sprites[0x86];
    GfxSprite *bottom = self->sprites[0x85];
    float angle;
    float t;
    float to[4] __attribute__((aligned(16)));
    float to2[4] __attribute__((aligned(16)));

    switch (self->finishState) {
    case 0:
        return;
    case 1:
        self->finishHold = 0.0f;
        self->finishAngle = 0.0f;
        if (SaveProfileGetWord(SaveGetProfile(), 7) == 0) {
            top->flags = top->flags & ~1u;
        } else {
            top->alpha = 0.0f;
            top->flags = top->flags | 1;
        }
        BtlHudFinishPairCopyQ(&top->posX, self->finishTarget);
        top->posX = top->posX - -300.0f;
        bottom->flags = bottom->flags | 1;
        bottom->alpha = 0.0f;
        BtlHudFinishPairCopyQ(&bottom->posX, self->finishTarget2);
        bottom->posX = bottom->posX - -300.0f;
        self->finishState = self->finishState + 1;
        /* fall through */
    case 2:
        angle = self->finishAngle + 0.04363323f;
        self->finishAngle = angle;
        angle = BtlHudFinishPairClamp(angle);
        self->finishAngle = angle;
        t = __builtin_sinf(angle);
        top->alpha = t;
        BtlHudFinishPairLerp(&top->posX, self->finishTarget, t);
        bottom->alpha = t;
        BtlHudFinishPairLerp(&bottom->posX, self->finishTarget2, t);
        if (top->alpha < 0.949999988f) {
            return;
        }
        top->alpha = 1.0f;
        BtlHudFinishPairCopyQ(&top->posX, self->finishTarget);
        bottom->alpha = 1.0f;
        BtlHudFinishPairCopyQ(&bottom->posX, self->finishTarget2);
        self->finishHold = 60.0f;
        self->finishState = self->finishState + 1;
        /* fall through */
    case 3:
        self->finishHold = self->finishHold + -1.0f;
        if (!(self->finishHold <= 0.0f)) {
            return;
        }
        self->finishAngle = BANNER_HALF_PI;
        self->finishState = 10;
        /* fall through */
    case 10:
        BtlHudFinishPairCopyQ(to, self->finishTarget);
        to[0] = to[0] + -300.0f;
        BtlHudFinishPairCopyQ(to2, self->finishTarget2);
        to2[0] = to2[0] + -300.0f;
        angle = self->finishAngle + -0.104719758f;
        self->finishAngle = angle;
        angle = BtlHudFinishPairClamp(angle);
        self->finishAngle = angle;
        top->alpha = angle;
        t = 1.0f - angle;
        BtlHudFinishPairLerp(&top->posX, to, t);
        bottom->alpha = angle;
        BtlHudFinishPairLerp(&bottom->posX, to2, t);
        if (!(top->alpha <= 0.0500000007f)) {
            return;
        }
        top->alpha = 0.0f;
        top->flags = top->flags & ~1u;
        bottom->alpha = 0.0f;
        bottom->flags = bottom->flags & ~1u;
        self->finishState = self->finishState + 1;
        /* fall through */
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    default:
        self->finishState = 0;
        return;
    }
}
