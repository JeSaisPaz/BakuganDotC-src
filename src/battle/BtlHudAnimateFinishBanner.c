// bdc 0x0882dbc8 BtlHudAnimateFinishBanner
#include "bdc.h"

/* End-of-battle banner animation (mode 0) of the battle HUD, driven by finishState. 1: shows
   `sprite` (flags bit 0, alpha 0) at finishTarget shifted 300 px right, then runs step 2 in the
   same frame. 2: finishAngle grows 2.5 degrees per frame, clamped to [0, pi/2]; alpha = angle and
   the position moves that fraction of the way to finishTarget; once alpha >= 0.95 it snaps to the
   target (alpha 1), arms a 60-frame hold and runs step 3. 3: counts finishHold down by 1; once it
   is <= 0 the angle is reset to pi/2 and step 10 runs. 10: the angle shrinks 6 degrees per frame
   (same clamp), alpha = angle and the position moves (1 - angle) of the way to finishTarget
   shifted 300 px left; once alpha <= 0.05 the sprite is hidden (alpha 0, flags bit 0 cleared) and
   the state returns to 0. States 4..9 and out-of-range ones reset to 0; 0 does nothing. */

#define BANNER_HALF_PI 1.57079637f

/* Lerp `pos += (to - pos) * t` on the sprite's position quad (posX..posW). */
static inline void BtlHudFinishBannerLerp(GfxSprite *sprite, const float *to, float t)
{
    float x = sprite->posX;
    float y = sprite->posY;
    float z = sprite->posZ;
    float w = sprite->posW;

    sprite->posX = x + (to[0] - x) * t;
    sprite->posY = y + (to[1] - y) * t;
    sprite->posZ = z + (to[2] - z) * t;
    sprite->posW = w + (to[3] - w) * t;
}

/* Quad copy of `src` into the sprite's position (posX..posW). */
static inline void BtlHudFinishBannerCopy(GfxSprite *sprite, const float *src)
{
    sprite->posX = src[0];
    sprite->posY = src[1];
    sprite->posZ = src[2];
    sprite->posW = src[3];
}

static inline float BtlHudFinishBannerClamp(float angle)
{
    if (angle < 0.0f) {
        return 0.0f;
    }
    if (!(angle <= BANNER_HALF_PI)) {
        return BANNER_HALF_PI;
    }
    return angle;
}

void BtlHudAnimateFinishBanner(BtlHud *self, GfxSprite *sprite)
{
    float angle;
    float to[4];

    switch (self->finishState) {
    case 0:
        return;
    case 1:
        self->finishHold = 0.0f;
        self->finishAngle = 0.0f;
        sprite->flags = sprite->flags | 1;
        sprite->alpha = 0.0f;
        BtlHudFinishBannerCopy(sprite, self->finishTarget);
        sprite->posX = sprite->posX - -300.0f;
        self->finishState = self->finishState + 1;
        /* fall through */
    case 2:
        angle = self->finishAngle + 0.04363323f;
        self->finishAngle = angle;
        angle = BtlHudFinishBannerClamp(angle);
        self->finishAngle = angle;
        sprite->alpha = angle;
        BtlHudFinishBannerLerp(sprite, self->finishTarget, angle);
        if (sprite->alpha < 0.949999988f) {
            return;
        }
        sprite->alpha = 1.0f;
        BtlHudFinishBannerCopy(sprite, self->finishTarget);
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
        to[0] = self->finishTarget[0];
        to[1] = self->finishTarget[1];
        to[2] = self->finishTarget[2];
        to[3] = self->finishTarget[3];
        to[0] = to[0] + -300.0f;
        angle = self->finishAngle + -0.104719758f;
        self->finishAngle = angle;
        angle = BtlHudFinishBannerClamp(angle);
        self->finishAngle = angle;
        sprite->alpha = angle;
        BtlHudFinishBannerLerp(sprite, to, 1.0f - angle);
        if (!(sprite->alpha <= 0.0500000007f)) {
            return;
        }
        sprite->alpha = 0.0f;
        sprite->flags = sprite->flags & ~1u;
        self->finishState = self->finishState + 1;
        /* fall through */
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        self->finishState = 0;
        return;
    default:
        self->finishState = 0;
        return;
    }
}
