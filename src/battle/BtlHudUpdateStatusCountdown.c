// bdc 0x08833e84 BtlHudUpdateStatusCountdown
#include "bdc.h"

/* HUD widget updater (`BtlHudPhaseMain`) for the countdown of the player Bakugan
   (`BtlHudGetPlayerBakugan`): shows its `respawnCountdown` (frames; with a pulsing digit, sprite
   `sprites[0xc5]`, cell `(frames / 30 + 1) % 10` laid out 5 per column) or, when that is 0, its
   `statusTimer` (digit sprite left hidden); sprite `sprites[0x5f]` toggles between cells 0 and 1
   every 10 frames and sprite `sprites[0xba]` is shown with it. When both timers are 0 the three
   sprites are hidden and `countdownState` resets to 0; a state other than 0/1 does nothing.
   The glow colour is `g_colorYellow` clamped to [0, 1] and scaled by 255 (VFPU bank constant S701);
   the sines are `vsin` of the phase times the bank's 2/pi (S703), i.e. sin of the phase. */

#define TWO_PI      6.28318548f   /* 0x40c90fdb */
#define HALF_PI     1.57079637f   /* 0x3fc90fdb */
#define GLOW_STEP   0.139626339f  /* 0x3e0efa35, 8 degrees */
#define SCALE_STEP  0.209439516f  /* 0x3e567750, 12 degrees */

void BtlHudUpdateStatusCountdown(BtlHud *self)
{
    ScePspFVector4 colour;
    BtlBakugan *unit;
    GfxSprite *digit;
    GfxSprite *blink;
    GfxSprite *frame;
    s32 timer;
    bool showDigit;
    s32 i;
    u8 alpha;
    float glow;
    float scale;

    unit = (BtlBakugan *)BtlHudGetPlayerBakugan(self);
    digit = self->sprites[0xc5];
    blink = self->sprites[0x5f];
    frame = self->sprites[0xba];
    if (unit == NULL) {
        return;
    }

    timer = 0;
    showDigit = false;
    if (unit->respawnCountdown != 0) {
        timer = unit->respawnCountdown;
        showDigit = true;
    } else if (unit->statusTimer != 0) {
        timer = unit->statusTimer;
    }

    if (timer == 0) {
        digit->flags &= ~1u;
        blink->flags &= ~1u;
        frame->flags &= ~1u;
        self->countdownState = 0;
        return;
    }

    if (self->countdownState > 0) {
        if (self->countdownState >= 2) {
            return;
        }
    } else {
        if (self->countdownState < 0) {
            return;
        }
        /* state 0: initialise and show */
        for (i = 0; i < 3; i++) {
            memset(self->countdownGlow[i], 0, sizeof(self->countdownGlow[i]));
        }
        self->countdownGlowPhase = 0.0f;
        self->countdownScalePhase = HALF_PI;
        if (showDigit) {
            digit->flags |= 1;
        }
        blink->flags |= 1;
        frame->flags |= 1;
        self->countdownBlinkTimer = 0;
        self->countdownBlinkCell = 0;
        self->countdownState = self->countdownState + 1;
    }

    /* state 1: animate */
    self->countdownGlowPhase = self->countdownGlowPhase + GLOW_STEP;
    if (!(self->countdownGlowPhase < TWO_PI)) {
        self->countdownGlowPhase = self->countdownGlowPhase - TWO_PI;
    }
    self->countdownScalePhase = self->countdownScalePhase + SCALE_STEP;
    if (!(self->countdownScalePhase < TWO_PI)) {
        self->countdownScalePhase = self->countdownScalePhase - TWO_PI;
    }

    glow = __builtin_sinf(self->countdownGlowPhase) * 0.1f + 0.15f;
    colour = g_colorYellow;
    alpha = (u8)(s32)(glow * 255.0f);
    for (i = 0; i < 3; i++) {
        self->countdownGlow[i][0] = VfI2uc(VfF2iz(VfSat0(colour.x) * 255.0f, 23));
        self->countdownGlow[i][1] = VfI2uc(VfF2iz(VfSat0(colour.y) * 255.0f, 23));
        self->countdownGlow[i][2] = VfI2uc(VfF2iz(VfSat0(colour.z) * 255.0f, 23));
        self->countdownGlow[i][3] = (i & 1) ? 0 : alpha;
    }

    scale = __builtin_sinf(self->countdownScalePhase) * 0.25f + 0.9f;
    timer = (timer / 30 + 1) % 10;
    GfxSpriteSetCell(digit, (float)(timer / 5), (float)(timer % 5));
    GfxSpriteSetScaleRotation(digit, scale, scale, 0.0f, false);

    if (!(self->countdownBlinkTimer++ < 9)) {
        self->countdownBlinkCell = self->countdownBlinkCell + 1;
        self->countdownBlinkCell = self->countdownBlinkCell % 2;
        self->countdownBlinkTimer = 0;
    }
    GfxSpriteSetCell(blink, (float)self->countdownBlinkCell, 0.0f);
}
