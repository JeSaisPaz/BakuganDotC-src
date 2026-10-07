// bdc 0x08844508 BtlHudPhaseResultDraw
#include "bdc.h"

/* HUD phase 7 (UI window 6, result kind 4): as `BtlHudPhaseResultWin` with banner sprite 4
   sliding in from -360 px, a 15-frame wait and voice 0x2a00. */

void BtlHudPhaseResultDraw(BtlHud *self)
{
    GfxSprite *banner;
    float t;
    float c;
    int step;

    BtlHudUpdateResultScreen(self);
    step = self->phaseStep;
    banner = self->resultSprites[4];
    if (step < 0) {
        return;
    }
    if (step < 2) {
        if (step == 0) {
            self->resultSprites[2]->flags &= ~1u;
            self->sprites[0xf0]->flags &= ~1u;
            /* save the banner's position quad (posX..posW -> resultBannerX..W) */
            self->resultBannerX = banner->posX;
            self->resultBannerY = banner->posY;
            self->resultBannerZ = banner->posZ;
            self->resultBannerW = banner->posW;
            /* the draw banner takes the win banner's Y */
            banner->posY = self->resultSprites[0]->posY;
            self->resultBannerY = banner->posY;
            self->resultSlideX = -360;
            self->resultTimer = 0;
            banner->flags |= 1;
            banner->alpha = 0.0f;
            self->phaseStep++;
        }
        self->resultTimer++;
        t = (float)self->resultTimer * 0.0416666679f - 1.0f;
        /* vmul by S703 (2/pi) then vcos (quarter turns): the cosine of the angle in radians */
        c = __builtin_cosf((1.0f - t * t) * 3.14159274f);
        banner->posX = self->resultBannerX +
                       (float)self->resultSlideX * (1.0f - (1.0f - c) * 0.5f);
        banner->alpha = banner->alpha + 0.05f;
        if (!(banner->alpha < 1.0f)) {
            banner->alpha = 1.0f;
        }
        if (self->resultTimer >= 20) {
            banner->posX = self->resultBannerX;
            self->resultWait = 15;
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
    BtlHudPlayVoice(self, 0x2a00);
    self->phase = 9;
    self->phaseStep = 0;
}
