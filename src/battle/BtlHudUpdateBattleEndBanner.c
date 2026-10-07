// bdc 0x0883fcb8 BtlHudUpdateBattleEndBanner
#include "bdc.h"

/* Battle-end banner of the HUD (sprite 2 of `resultSprites`), stepped by `endBannerState`. When UI
   window 3 opens for the first time (`bannerClosing`) the state jumps to 10. In rule mode 2
   (script variable 8) with at least 3 rounds (profile word 0x1b clamped to 1..5), while the camera
   task exists and `BtlMainIsMatchUndecided` holds, it also drives `BtlHudUpdateRoundWinLamps`.
   State 0 plays voice 0x29fd (`SndBgmPlayVoice`) unless the outcome is 2 outside profile flag 0
   mode, shows the sprite, centres its pivot, resets its scale and insets its UV by half a texel;
   states 0..1 fade it in with a cosine ease over 20 frames (`resultWait`), then hold (state 2).
   State 10 resets the timers; states 10..11 fade it out over 25 frames while scaling it by a sine
   of `bannerZoom` (+1.5 per frame) horizontally and the fade vertically, moving on once alpha
   reaches 0. Every frame, the sprite is hidden while the outcome is 2 outside profile flag 0 mode.
   The cosine/sine arguments are radians (scaled by the bank constant S703 = 2/pi for the VFPU). */

void BtlHudUpdateBattleEndBanner(BtlHud *self)
{
    GfxSprite *sprite = self->resultSprites[2];
    s32 rounds;
    float t;
    float c;
    float s;
    float fade;
    float zoom;

    if (UiGetWindowActive(3) == 1) {
        if (self->bannerClosing == 0) {
            self->endBannerState = 10;
        }
        self->bannerClosing = 1;
    }
    rounds = (s32)SaveProfileGetWord(SaveGetProfile(), 0x1b);
    if (rounds < 1) {
        rounds = 1;
    } else if (rounds > 5) {
        rounds = 5;
    }
    if (BtlCameraTaskExists() != 0 && g_scriptGlobalVars[8] == 2 && rounds >= 3 &&
        BtlMainIsMatchUndecided(BtlGetCameraTask())) {
        BtlHudUpdateRoundWinLamps(self);
    }

    switch (self->endBannerState) {
    case 0:
    case 1:
        if (self->endBannerState == 0) {
            if (SaveGetProfileFlag0() != 0 || g_btlBattleOutcome != 2) {
                SndBgmPlayVoice(0x29fd);
            }
            sprite->flags |= 1;
            GfxSpriteCenterPivot(sprite);
            GfxSpriteSetScaleRotation(sprite, 1.0f, 1.0f, 0.0f, false);
            GfxSpriteInsetUv(0.5f, sprite);
            self->bannerZoom = 0.0f;
            self->resultWait = 0;
            self->endBannerState++;
            self->resultWait = 1;
        } else {
            self->resultWait++;
        }
        t = (float)self->resultWait * 0.0500000007f - 1.0f;
        c = __builtin_cosf((1.0f - t * t) * 3.14159274f);
        fade = (1.0f - c) * 0.5f;
        sprite->alpha = fade;
        if (!(fade < 1.0f)) {
            sprite->alpha = 1.0f;
            self->endBannerState++;
        }
        break;
    case 10:
    case 11:
        if (self->endBannerState == 10) {
            self->bannerZoom = 0.0f;
            self->resultWait = 0;
            self->endBannerState++;
            self->resultWait = 1;
        } else {
            self->resultWait++;
        }
        t = (float)self->resultWait * 0.0399999991f - 1.0f;
        c = __builtin_cosf((1.0f - t * t) * 3.14159274f);
        fade = (1.0f - c) * 0.5f;
        sprite->alpha = 1.0f - fade;
        zoom = self->bannerZoom;
        t = zoom * 0.0399999991f - 1.0f;
        s = __builtin_sinf(1.0f - t * t);
        self->bannerZoom = zoom + 1.5f;
        GfxSpriteSetScaleRotation(sprite, s * 0.800000012f + 1.0f, fade * 0.600000024f + 1.0f,
                                  0.0f, false);
        if (sprite->alpha <= 0.0f) {
            sprite->alpha = 0.0f;
            self->endBannerState++;
        }
        break;
    default:
        break;
    }

    if (SaveGetProfileFlag0() == 0 && g_btlBattleOutcome == 2) {
        sprite->flags &= ~1u;
    }
}
