// bdc 0x0883a20c BtlHudUpdateArtReadyFlash
#include "bdc.h"

/* HUD widget updater (`BtlHudPhaseMain`) for the special-art "ready" lamps. With no player
   Bakugan it does nothing. Otherwise it ping-pongs `artFlashAlpha` by 0.05 per call between 0 and
   1.5 (`artFlashDir` 0 = rising, clamped and flipped once it is not below 1.5; 1 = falling,
   clamped and flipped once it is <= 0), writes it to the alpha of HUD sprites 0xb0, 0xb2, 0xb1 and
   0xb5, then sets the visible bit (flags bit 0) of sprite 0xb0 / 0xb1 when art slot 0 / 1's
   charge times 0.0001 is not below 1.0 (charge >= 10000) and clears it otherwise; sprites 0xb2 and
   0xb5 always have it cleared. */
void BtlHudUpdateArtReadyFlash(BtlHud *self)
{
    BtlBakugan *bakugan;
    float alpha;

    bakugan = BtlHudGetPlayerBakugan(self);
    if (bakugan == NULL) {
        return;
    }
    if (self->artFlashDir == 0) {
        alpha = self->artFlashAlpha + 0.0500000007f;
        self->artFlashAlpha = alpha;
        if (!(alpha < 1.5f)) {
            alpha = 1.5f;
            self->artFlashAlpha = 1.5f;
            self->artFlashDir = 1;
        }
    } else {
        alpha = self->artFlashAlpha - 0.0500000007f;
        self->artFlashAlpha = alpha;
        if (alpha <= 0.0f) {
            self->artFlashAlpha = 0.0f;
            alpha = 0.0f;
            self->artFlashDir = 0;
        }
    }
    self->sprites[0xb0]->alpha = alpha;
    self->sprites[0xb2]->alpha = alpha;
    self->sprites[0xb1]->alpha = alpha;
    self->sprites[0xb5]->alpha = alpha;

    if (!(bakugan->combat.artCharge[0] * 9.99999975e-05f < 1.0f)) {
        self->sprites[0xb0]->flags |= 1;
    } else {
        self->sprites[0xb0]->flags &= ~1u;
    }
    self->sprites[0xb2]->flags &= ~1u;

    if (!(bakugan->combat.artCharge[1] * 9.99999975e-05f < 1.0f)) {
        self->sprites[0xb1]->flags |= 1;
    } else {
        self->sprites[0xb1]->flags &= ~1u;
    }
    self->sprites[0xb5]->flags &= ~1u;
}
