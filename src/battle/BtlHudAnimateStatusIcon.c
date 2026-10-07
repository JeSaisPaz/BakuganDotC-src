// bdc 0x0882d6f0 BtlHudAnimateStatusIcon
#include "bdc.h"

/* Animates status icon `i` (sprite `sprites[0xe9 + i]`, byte offset `0x3a4 + 4i` into the HUD
   layout sprite array) of the battle HUD (`BtlHudUpdate`); state machine `iconState[i]`: when
   `active` becomes true the icon fades in (sine ease, ~8° per frame) at slot position x = 76 +
   24·n (n = `iconCount`, icons shown so far), y = 259, slides to a new slot when the count
   changes, and fades out when `active` goes false. The ease is sin(angle): vsin.s of
   angle times the bank's S703 = 2/pi. */

#define FADE_IN_STEP   0.13962634f    /* 8 degrees */
#define FADE_OUT_STEP  -0.20943952f   /* -12 degrees */
#define HALF_PI        1.5707964f

/* pos += (target - pos) * t, all four lanes (vsub.q, vscl.q, vadd.q). */
static inline void Vec4Lerp(float *pos, const float *target, float t)
{
    s32 k;

    for (k = 0; k < 4; k++) {
        pos[k] = pos[k] + (target[k] - pos[k]) * t;
    }
}

/* 16-byte copy (lv.q/sv.q). */
static inline void Vec4Copy(float *out, const float *src)
{
    out[0] = src[0];
    out[1] = src[1];
    out[2] = src[2];
    out[3] = src[3];
}

/* Adds `step` to the icon's ease angle, clamps it to [0, pi/2] and stores it. */
static inline float StepAngle(float *slot, float step)
{
    float angle = *slot + step;

    *slot = angle;
    if (angle < 0.0f) {
        angle = 0.0f;
    } else if (!(angle <= HALF_PI)) {
        angle = HALF_PI;
    }
    *slot = angle;
    return angle;
}

void BtlHudAnimateStatusIcon(BtlHud *hud, s32 i, bool active)
{
    GfxSprite *sprite = hud->sprites[0x3a4 / 4 + i];
    s32 *state = &hud->iconState[i];
    s32 *slot = &hud->iconSlot[i];
    float *angle = &hud->iconAngle[i];
    float *target = hud->iconTarget[i];
    float value;

    switch (*state) {
    case 0: /* hidden: wait for the condition */
        if (!active) {
            return;
        }
        (*state)++;
        /* fall through */
    case 1: /* show at the next free slot */
        hud->iconAux[i] = 0;
        *angle = 0.0f;
        *slot = hud->iconCount;
        target[0] = (float)hud->iconCount * 24.0f + 76.0f;
        target[1] = 259.0f;
        target[2] = 168.0f;
        sprite->flags |= 1;
        sprite->alpha = 0.0f;
        Vec4Copy(&sprite->posX, target);
        (*state)++;
        /* fall through */
    case 2: /* fade in */
        value = __builtin_sinf(StepAngle(angle, FADE_IN_STEP));
        sprite->alpha = value;
        if (value < 0.95f) {
            return;
        }
        sprite->alpha = 1.0f;
        (*state)++;
        /* fall through */
    case 3: /* move to the current slot */
        *angle = 0.0f;
        *slot = hud->iconCount;
        (*state)++;
        target[0] = (float)*slot * 24.0f + 76.0f;
        /* fall through */
    case 4: /* slide */
        Vec4Lerp(&sprite->posX, target, __builtin_sinf(StepAngle(angle, FADE_IN_STEP)));
        if (*angle < HALF_PI) {
            return;
        }
        Vec4Copy(&sprite->posX, target);
        (*state)++;
        /* fall through */
    case 5: /* shown: re-slot when the count changes, fade out when the condition ends */
        if (active) {
            if (*slot != hud->iconCount) {
                *state = 3;
            }
            return;
        }
        *angle = HALF_PI;
        *state = 10;
        /* fall through */
    case 10: /* fade out */
        if (active) {
            *state = 2;
            return;
        }
        value = __builtin_sinf(StepAngle(angle, FADE_OUT_STEP));
        sprite->alpha = value;
        if (!(value <= 0.05f)) {
            return;
        }
        sprite->alpha = 0.0f;
        sprite->flags &= ~1u;
        (*state)++;
        *state = 0;
        return;
    default: /* 6..9 and out of range */
        *state = 0;
        return;
    }
}
