// bdc 0x08836d1c BtlHudUpdateMultiRoundBanner
#include "bdc.h"

/* HUD widget updater (`BtlHudPhaseMain`) for the 3-sprite multi-round banner
   (`sprites[0x8e..0x90]`, consecutive GfxSprite objects) driven by `multiRound`:
   state 1 sets a 15-frame delay (`multiRoundHold`), 2 counts it down; 3 shows the sprites with
   alpha 0 at their saved positions (`multiRoundPos`) with posY + 16, and when `multiRoundSkip`
   is set and the language is 3 or 12 hides the first two; 4 raises `multiRoundFade` by 5 degrees
   (in radians) per frame, clamped to [0, pi/2], uses it as alpha and as the step of an
   approach towards the saved positions until the first sprite's alpha reaches 0.95, then snaps
   them there with alpha 1 and holds for 105 frames (state 5); state 10 lowers the fade by 15
   degrees per frame and moves the sprites towards their saved positions with posY - 16 (step
   1 - fade), and once the first sprite's alpha is <= 0.05 hides all three and resets
   `multiRound` to 0 (also done for states 6-9 and > 10). The sprite left untouched in states 4
   and 10 is the third one, or the second when `multiRoundSkip` is set. */

#define MULTI_ROUND_SPRITE  0x8e        /* first banner sprite: sprites[0x238 / 4] */
#define FADE_IN_STEP        0.0872664601f   /* 0x3db2b8c2, 5 degrees */
#define FADE_OUT_STEP       -0.261799395f   /* 0xbe860a92, -15 degrees */
#define HALF_PI             1.57079637f     /* 0x3fc90fdb */

/* Copy of a saved vec4 into the sprite's position (lv.q/sv.q). */
static inline void MultiRoundSetPos(GfxSprite *spr, const float *src)
{
    spr->posX = src[0];
    spr->posY = src[1];
    spr->posZ = src[2];
    spr->posW = src[3];
}

/* pos += (target - pos) * t, all four lanes (vsub.q/vscl.q/vadd.q). */
static inline void MultiRoundLerpPos(GfxSprite *spr, float tx, float ty, float tz, float tw, float t)
{
    spr->posX = spr->posX + (tx - spr->posX) * t;
    spr->posY = spr->posY + (ty - spr->posY) * t;
    spr->posZ = spr->posZ + (tz - spr->posZ) * t;
    spr->posW = spr->posW + (tw - spr->posW) * t;
}

/* Sprite index i (0..2) is updated in the fade states unless it is the skipped one. */
static inline int MultiRoundIsSkipped(BtlHud *self, int i)
{
    if (self->multiRoundSkip != 0) {
        return i == 1;
    }
    return i == 2;
}

void BtlHudUpdateMultiRoundBanner(BtlHud *self)
{
    GfxSprite *spr;
    float hold;
    float fade;
    s32 language;
    int i;
    const float *pos;

    switch (self->multiRound) {
    case 0:
        return;
    case 1:
        self->multiRoundHold = 15.0f;
        self->multiRound++;
        /* fall through */
    case 2:
        hold = self->multiRoundHold + -1.0f;
        self->multiRoundHold = hold;
        if (!(hold <= 0.0f)) {
            return;
        }
        self->multiRound++;
        /* fall through */
    case 3:
        self->multiRoundHold = 0.0f;
        self->multiRoundFade = 0.0f;
        spr = self->sprites[MULTI_ROUND_SPRITE];
        for (i = 0; i < 3; i++, spr++) {
            spr->flags |= 1;
            spr->alpha = 0.0f;
            MultiRoundSetPos(spr, self->multiRoundPos[i]);
            spr->posY = spr->posY + 16.0f;
        }
        self->multiRound++;
        if (self->multiRoundSkip != 0) {
            language = SaveProfileGetLanguage(SaveGetProfile());
            if (language == 12 || language == 3) {
                self->sprites[MULTI_ROUND_SPRITE]->flags &= ~1u;
                self->sprites[MULTI_ROUND_SPRITE + 1]->flags &= ~1u;
            }
        }
        /* fall through */
    case 4:
        fade = self->multiRoundFade + FADE_IN_STEP;
        self->multiRoundFade = fade;
        if (fade < 0.0f) {
            fade = 0.0f;
        } else if (!(fade <= HALF_PI)) {
            fade = HALF_PI;
        }
        self->multiRoundFade = fade;
        spr = self->sprites[MULTI_ROUND_SPRITE];
        for (i = 0; i < 3; i++, spr++) {
            if (MultiRoundIsSkipped(self, i)) {
                continue;
            }
            fade = self->multiRoundFade;
            spr->alpha = fade;
            pos = self->multiRoundPos[i];
            MultiRoundLerpPos(spr, pos[0], pos[1], pos[2], pos[3], fade);
        }
        if (self->sprites[MULTI_ROUND_SPRITE]->alpha < 0.95f) {
            return;
        }
        spr = self->sprites[MULTI_ROUND_SPRITE];
        for (i = 0; i < 3; i++, spr++) {
            if (MultiRoundIsSkipped(self, i)) {
                continue;
            }
            spr->alpha = 1.0f;
            MultiRoundSetPos(spr, self->multiRoundPos[i]);
        }
        self->multiRoundHold = 105.0f;
        self->multiRound++;
        /* fall through */
    case 5:
        hold = self->multiRoundHold + -1.0f;
        self->multiRoundHold = hold;
        if (!(hold <= 0.0f)) {
            return;
        }
        self->multiRoundHold = 40.0f;
        self->multiRoundFade = HALF_PI;
        self->multiRound = 10;
        /* fall through */
    case 10:
        fade = self->multiRoundFade + FADE_OUT_STEP;
        self->multiRoundFade = fade;
        if (fade < 0.0f) {
            fade = 0.0f;
        } else if (!(fade <= HALF_PI)) {
            fade = HALF_PI;
        }
        self->multiRoundFade = fade;
        spr = self->sprites[MULTI_ROUND_SPRITE];
        for (i = 0; i < 3; i++, spr++) {
            if (MultiRoundIsSkipped(self, i)) {
                continue;
            }
            pos = self->multiRoundPos[i];
            fade = self->multiRoundFade;
            spr->alpha = fade;
            MultiRoundLerpPos(spr, pos[0], pos[1] - 16.0f, pos[2], pos[3], 1.0f - fade);
        }
        if (!(self->sprites[MULTI_ROUND_SPRITE]->alpha <= 0.05f)) {
            return;
        }
        spr = self->sprites[MULTI_ROUND_SPRITE];
        for (i = 0; i < 3; i++, spr++) {
            spr->alpha = 0.0f;
            spr->flags &= ~1u;
        }
        self->multiRound++;
        /* fall through */
    case 6:
    case 7:
    case 8:
    case 9:
        self->multiRound = 0;
        return;
    default:
        self->multiRound = 0;
        return;
    }
}
