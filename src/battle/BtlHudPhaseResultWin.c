// bdc 0x08844110 BtlHudPhaseResultWin
#include "bdc.h"

/* HUD phase 5 (UI window 4, result kind 1): runs `BtlHudUpdateResultScreen`; step 0 hides
   result sprite 2 and HUD sprite 0xf0, saves win banner sprite 0's position quad and shows it
   transparent with a +360 px slide offset; steps 0..1 ease its x in from that offset with a
   cosine curve over the result timer (1/24 per frame) and fade it in by 0.05 per frame (capped at
   1); at timer 20 it snaps to the saved x and waits 5 frames (step 2); step 3 plays voice 0x29fe
   (`BtlHudPlayVoice`) and switches to phase 9. */

void BtlHudPhaseResultWin(BtlHud *self)
{
    GfxSprite *banner;
    float t;
    float c;
    int step;

    BtlHudUpdateResultScreen(self);
    step = self->phaseStep;
    banner = self->resultSprites[0];
    if (step < 0) {
        return;
    }
    if (step < 2) {
        if (step == 0) {
            self->resultSprites[2]->flags &= ~1u;
            self->sprites[0xf0]->flags &= ~1u;
            /* save the banner's position quad */
            self->resultBannerX = banner->posX;
            self->resultBannerY = banner->posY;
            self->resultBannerZ = banner->posZ;
            self->resultBannerW = banner->posW;
            self->resultSlideX = 360;
            self->resultTimer = 0;
            banner->flags |= 1;
            banner->alpha = 0.0f;
            self->phaseStep++;
        }
        self->resultTimer++;
        t = (float)self->resultTimer * 0.0416666679f - 1.0f;
        /* vcos.s of angle * S703 (2/pi): the cosine of the angle in radians */
        c = __builtin_cosf((1.0f - t * t) * 3.14159274f);
        banner->posX = self->resultBannerX +
                       (float)self->resultSlideX * (1.0f - (1.0f - c) * 0.5f);
        banner->alpha = banner->alpha + 0.05f;
        if (!(banner->alpha < 1.0f)) {
            banner->alpha = 1.0f;
        }
        if (self->resultTimer >= 20) {
            banner->posX = self->resultBannerX;
            self->resultWait = 5;
            self->phaseStep++;
        }
        return;
    }
    if (step == 2) {
        self->resultWait--;
        if (self->resultWait > 0) {
            return;
        }
        self->phaseStep++;
    } else if (step > 3) {
        return;
    }
    BtlHudPlayVoice(self, 0x29fe);
    self->phase = 9;
    self->phaseStep = 0;
}
