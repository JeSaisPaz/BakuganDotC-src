// bdc 0x08844bec BtlHudPhaseRoundEnd
#include "bdc.h"

/* HUD phase 8 (UI window 7, round end): runs `BtlHudUpdateResultScreen`, then steps through the
   round banner (result sprite 3). Step 0 plays voice 0x29fc (`SndBgmPlayVoice`), hides result
   sprite 2, saves the banner's position quad, sets a -360 px slide offset, zeroes the alpha of HUD
   sprite 0xf0 (dim) and shows the banner transparent. Steps 0..1 ease the banner's x in with a
   cosine curve over the result timer (1/24 per frame), fade it in by 0.05 per frame (capped at 1)
   with the dim sprite at 80 % of its alpha, and at timer 20 snap it to the saved x. Steps 2..3 hold
   for 30 frames. Steps 4..5 slide it out by +360 px with the same curve and fade it by 0.12 per
   frame; at alpha 0 the dim sprite is hidden and the step becomes 99. Step 99 closes window 7, opens
   window 8, returns to phase 4 running `BtlHudPhaseResultSelect` once, and picks phase 9 instead
   when the camera task exists and `BtlMainIsMatchUndecided` holds; the step is reset to 0. */
void BtlHudPhaseRoundEnd(BtlHud *self)
{
    GfxSprite *banner;
    float angle, c;
    float t;
    s32 step;

    BtlHudUpdateResultScreen(self);
    step = self->phaseStep;
    banner = self->resultSprites[3];
    if (step > 5) {
        if (step != 99) {
            return;
        }
        UiSetWindowActive(7, 0);
        UiSetWindowActive(8, 1);
        self->phase = 4;
        BtlHudPhaseResultSelect(self);
        if (!BtlCameraTaskExists()) {
            self->phaseStep = 0;
            return;
        }
        if (BtlMainIsMatchUndecided(BtlGetCameraTask())) {
            self->phase = 9;
        }
        self->phaseStep = 0;
        return;
    }
    if (step < 0) {
        return;
    }

    if (step <= 1) {
        if (step == 0) {
            SndBgmPlayVoice(0x29fc);
            self->resultSprites[2]->flags &= ~1u;
            self->resultBannerX = banner->posX;
            self->resultBannerY = banner->posY;
            self->resultBannerZ = banner->posZ;
            self->resultBannerW = banner->posW;
            self->resultSlideX = -360;
            self->resultTimer = 0;
            self->sprites[0xf0]->alpha = 0.0f;
            banner->flags |= 1;
            banner->alpha = 0.0f;
            self->phaseStep++;
        }
        self->resultTimer++;
        t = (float)self->resultTimer * 0.0416666679f - 1.0f;
        angle = (1.0f - t * t) * 3.14159274f;
        c = __builtin_cosf(angle);
        banner->posX = self->resultBannerX +
                       (float)self->resultSlideX * (1.0f - (1.0f - c) * 0.5f);
        banner->alpha = banner->alpha + 0.05f;
        self->sprites[0xf0]->alpha = banner->alpha * 0.800000012f;
        if (!(banner->alpha < 1.0f)) {
            banner->alpha = 1.0f;
        }
        if (self->resultTimer >= 20) {
            self->sprites[0xf0]->alpha = 0.800000012f;
            banner->posX = self->resultBannerX;
            self->phaseStep++;
        }
        return;
    }

    if (step <= 3) {
        if (step == 2) {
            self->resultTimer = 30;
            self->phaseStep++;
        }
        self->resultTimer--;
        if (self->resultTimer > 0) {
            return;
        }
        self->phaseStep++;
        step = 4;
    }
    if (step == 4) {
        self->resultSlideX = 360;
        self->resultTimer = 0;
        self->phaseStep++;
    }
    self->resultTimer++;
    t = (float)self->resultTimer * 0.0416666679f - 1.0f;
    angle = (1.0f - t * t) * 3.14159274f;
    c = __builtin_cosf(angle);
    banner->posX = self->resultBannerX + (float)self->resultSlideX * ((1.0f - c) * 0.5f);
    banner->alpha = banner->alpha - 0.119999997f;
    self->sprites[0xf0]->alpha = banner->alpha * 0.800000012f;
    if (banner->alpha <= 0.0f) {
        banner->alpha = 0.0f;
        self->sprites[0xf0]->flags &= ~1u;
        self->sprites[0xf0]->alpha = 0.0f;
        banner->posX = self->resultBannerX + (float)self->resultSlideX;
        self->phaseStep = 99;
    }
}
