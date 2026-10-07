// bdc 0x0882d32c BtlHudUpdateGuardPopup
#include "bdc.h"

/* Quad copy `sprite->pos = src` (lv.q/sv.q). */
static inline void BtlHudGuardSetPos(GfxSprite *sprite, const float *src)
{
    float x = src[0];
    float y = src[1];
    float z = src[2];
    float w = src[3];

    sprite->posX = x;
    sprite->posY = y;
    sprite->posZ = z;
    sprite->posW = w;
}

/* `pos += (target - pos) * k` on all four lanes (vsub.q, vscl.q, vadd.q). */
static inline void BtlHudGuardLerpPos(GfxSprite *sprite, const float *target, float k)
{
    float x = sprite->posX;
    float y = sprite->posY;
    float z = sprite->posZ;
    float w = sprite->posW;
    float dx = (target[0] - x) * k;
    float dy = (target[1] - y) * k;
    float dz = (target[2] - z) * k;
    float dw = (target[3] - w) * k;

    sprite->posX = x + dx;
    sprite->posY = y + dy;
    sprite->posZ = z + dz;
    sprite->posW = w + dw;
}

/* Animates the guard popup sprite (`sprites[0x75]`) of the battle HUD (`BtlHudUpdate`) for the
   player Bakugan `unit` (nothing when `unit` is NULL), driven by `guardState`. State 0 waits for
   bit 0x40 of the unit's `stateFlags` and then runs state 1 in the same frame. State 1 zeroes
   `guardTimer` and `guardFade`, shows the sprite at alpha 0 placed 32 px left of `guardRestPos`,
   hides `sprites[0x76]` (flag bit 0) and moves to state 2. State 2 adds 0.139626339 rad (8°) per
   frame to `guardFade` (clamped to [0, pi/2]), uses it as the alpha and moves the sprite that
   fraction of the way to `guardRestPos` (VFPU lerp); once alpha reaches 0.95 it snaps to alpha 1
   at the rest position, sets `guardTimer` to 15 and enters state 3 in the same frame. State 3
   decrements `guardTimer` each frame; when it is no longer positive `guardFade` becomes pi/2 and
   state 10 runs in the same frame. State 10 subtracts 0.157079637 rad (9°) per frame from
   `guardFade` (same clamp), uses it as the alpha and moves the sprite `1 - guardFade` of the way
   towards 32 px right of the rest position; once alpha is at most 0.05 it sets alpha 0, hides the
   sprite and returns to state 0. Any other state is reset to 0. */
void BtlHudUpdateGuardPopup(BtlHud *self, BtlBakugan *unit)
{
    GfxSprite *sprite;
    GfxSprite *linkSprite;
    float fade;
    float scale;
    float target[4];

    if (unit == NULL) {
        return;
    }
    sprite = self->sprites[0x75];
    switch (self->guardState) {
    case 0:
        if ((unit->stateFlags & 0x40) == 0) {
            return;
        }
        self->guardState++;
        /* fall through */
    case 1:
        self->guardTimer = 0;
        self->guardFade = 0.0f;
        sprite->flags |= 1;
        sprite->alpha = 0.0f;
        BtlHudGuardSetPos(sprite, self->guardRestPos);
        sprite->posX = sprite->posX - 32.0f;
        self->guardState++;
        linkSprite = self->sprites[0x76];
        linkSprite->flags &= ~1u;
        /* fall through */
    case 2:
        fade = self->guardFade + 0.139626339f;
        self->guardFade = fade;
        if (fade < 0.0f) {
            fade = 0.0f;
        } else if (!(fade <= 1.57079637f)) {
            fade = 1.57079637f;
        }
        self->guardFade = fade;
        sprite->alpha = fade;
        /* pos += (guardRestPos - pos) * fade */
        BtlHudGuardLerpPos(sprite, self->guardRestPos, fade);
        if (sprite->alpha < 0.949999988f) {
            return;
        }
        sprite->alpha = 1.0f;
        BtlHudGuardSetPos(sprite, self->guardRestPos);
        self->guardTimer = 15;
        self->guardState++;
        /* fall through */
    case 3:
        self->guardTimer--;
        if (self->guardTimer > 0) {
            return;
        }
        self->guardFade = 1.57079637f;
        self->guardState = 10;
        /* fall through */
    case 10:
        target[0] = self->guardRestPos[0];
        target[1] = self->guardRestPos[1];
        target[2] = self->guardRestPos[2];
        target[3] = self->guardRestPos[3];
        target[0] = target[0] + 32.0f;
        fade = self->guardFade + -0.157079637f;
        self->guardFade = fade;
        if (fade < 0.0f) {
            fade = 0.0f;
        } else if (!(fade <= 1.57079637f)) {
            fade = 1.57079637f;
        }
        self->guardFade = fade;
        sprite->alpha = fade;
        scale = 1.0f - fade;
        /* pos += (target - pos) * (1 - fade) */
        BtlHudGuardLerpPos(sprite, target, scale);
        if (!(sprite->alpha <= 0.0500000007f)) {
            return;
        }
        sprite->alpha = 0.0f;
        sprite->flags &= ~1u;
        self->guardState++;
        self->guardState = 0;
        break;
    default:
        self->guardState = 0;
        break;
    }
}
