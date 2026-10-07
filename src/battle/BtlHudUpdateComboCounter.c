// bdc 0x0882f5a0 BtlHudUpdateComboCounter
#include "bdc.h"

/* Combo counter widget of the battle HUD (`BtlHudUpdate`) (`BtlHudPhaseMain`), state
   `comboState`: shows the two-digit hit count from `BtlHudGetComboCount` (sprites 0–2 of the
   HUD layout sprite array: tens digit, ones digit, and sprite 2 which never gets a cell, likely
   the label; clamped to 0..99, the tens digit only
   when non-zero) once it reaches 2. Each new hit restarts a 3-frame pulse (scale
   `1 + 0.2 sin(comboAngle)`, comboAngle +32° per frame, a random jitter of up to 1 px around
   `comboDigitPos`) and remembers the best combo in `bestCombo`; then the digits snap back to
   scale 1 and their rest positions. When the count drops to 0 the counter fades out
   (alpha `sin(comboFade)`, -15° per frame) with a slight zoom (`1 + 0.15 sin(comboAngle)`,
   +15° per frame), then resets; a new combo during the fade resets it at once.
   States: 0 init (hide, scale 1) → 1 wait for count ≥ 2 → 2 start pulse → 3 pulse → 4 rest →
   5 hold → 10/11 fade → 20 → 0. 6–9 and 12–19 idle. Whenever the state ends > 0 the digit cells
   are rewritten from `comboCount` (`GfxSpriteSetCell`, cell = digit / 5, digit % 5).
   Bank constants S703 (2/pi, so `vsin.s` takes radians), S733 = 1, S700 = 2pi, S702 = pi (map
   `vrndf1.s` to the jitter angle `(r - 1) * 2pi - pi`) are literals. */

#define PULSE_STEP  0.558505356f   /* 0x3f0efa35, 32 degrees */
#define TWO_PI      6.28318548f    /* 0x40c90fdb */
#define PULSE_AMP   0.200000003f   /* 0x3e4ccccd */
#define HALF_PI     1.57079637f    /* 0x3fc90fdb */
#define FADE_STEP   0.261799395f   /* 0x3e860a92, 15 degrees */
#define ZOOM_AMP    0.150000006f   /* 0x3e19999a */

/* sin of a random angle in [-pi, pi): vrndf1 (in [1, 2)) mapped by the bank as
   ((r - 1) * 2pi - pi), then vsin of that times 2/pi quarter turns. */
static inline float ComboJitter(void)
{
    return __builtin_sinf((PlatformRandFloat12() - 1.0f) * 6.28318548f - 3.14159274f);
}

void BtlHudUpdateComboCounter(BtlHud *self)
{
    GfxSprite *sprite;
    s32 count;
    s32 best;
    s32 digit;
    s32 i;
    s32 pulseDone;
    float angle;
    float scale;
    float alpha;
    float jitter;

    switch (self->comboState) {
    case 0:
        for (i = 0; i < 3; i++) {
            sprite = self->sprites[i];
            GfxSpriteSetBottomCentrePivot(sprite);
            sprite->alpha = 1.0f;
            GfxSpriteSetScaleRotation(sprite, 1.0f, 1.0f, 0.0f, false);
            sprite->flags &= ~1u;
        }
        self->comboSpare0fc = 0;
        self->comboCount = 0;
        self->comboSpare104 = 0;
        self->comboPulseFrames = 0;
        self->comboSpare110 = 0.0f;
        self->comboSpare108 = 0.0f;
        self->comboState++;
        /* fall through */
    case 1:
        count = BtlHudGetComboCount(self);
        if (self->comboCount == count || count < 2) {
            break;
        }
        self->sprites[1]->flags |= 1;
        self->sprites[2]->flags |= 1;
        self->comboCount = count;
        self->comboState++;
        /* fall through */
    case 2:
        if (self->bestCombo < self->comboCount) {
            best = self->comboCount;
        } else {
            best = self->bestCombo;
        }
        self->bestCombo = best;
        self->comboSpare110 = 0.0f;
        self->comboAngle = 0.0f;
        self->comboPulseFrames = 0;
        self->comboState++;
        /* fall through */
    case 3:
        pulseDone = 0;
        count = BtlHudGetComboCount(self);
        self->comboAngle = self->comboAngle + PULSE_STEP;
        if (!(self->comboAngle <= TWO_PI)) {
            self->comboAngle = self->comboAngle - TWO_PI;
        }
        scale = __builtin_sinf(self->comboAngle) * PULSE_AMP + 1.0f;
        self->comboPulseFrames++;
        if (self->comboPulseFrames >= 3) {
            pulseDone = 1;
        }
        for (i = 0; i < 3; i++) {
            sprite = self->sprites[i];
            GfxSpriteSetScaleRotation(sprite, scale, scale, 0.0f, false);
            jitter = ComboJitter();
            sprite->posX = self->comboDigitPos[i][0] + jitter;
            jitter = ComboJitter();
            sprite->posY = self->comboDigitPos[i][1] + jitter;
        }
        if (self->comboCount < count) {
            self->comboCount = count;
            self->comboState = 2;
            break;
        }
        if (!pulseDone) {
            break;
        }
        self->comboState++;
        /* fall through */
    case 4:
        self->comboAngle = 1.0f;
        for (i = 0; i < 3; i++) {
            sprite = self->sprites[i];
            GfxSpriteSetScaleRotation(sprite, self->comboAngle, self->comboAngle, 0.0f, false);
            sprite->posX = self->comboDigitPos[i][0];
            sprite->posY = self->comboDigitPos[i][1];
            sprite->posZ = self->comboDigitPos[i][2];
            sprite->posW = self->comboDigitPos[i][3];
        }
        self->comboState++;
        /* fall through */
    case 5:
        count = BtlHudGetComboCount(self);
        if (self->comboCount < count) {
            self->comboCount = count;
            self->comboState = 2;
        } else if (count == 0) {
            self->comboState = 10;
        }
        break;
    case 10:
        self->comboFade = HALF_PI;
        self->comboAngle = 0.0f;
        self->comboState++;
        /* fall through */
    case 11:
        if (BtlHudGetComboCount(self) != 0) {
            self->comboState = 0;
            break;
        }
        angle = self->comboFade + -FADE_STEP;
        self->comboFade = angle;
        if (angle < 0.0f) {
            angle = 0.0f;
        } else if (!(angle <= HALF_PI)) {
            angle = HALF_PI;
        }
        self->comboFade = angle;
        alpha = __builtin_sinf(angle);
        angle = self->comboAngle + FADE_STEP;
        self->comboAngle = angle;
        if (angle < 0.0f) {
            angle = 0.0f;
        } else if (!(angle <= HALF_PI)) {
            angle = HALF_PI;
        }
        self->comboAngle = angle;
        scale = __builtin_sinf(angle) * ZOOM_AMP + 1.0f;
        for (i = 0; i < 3; i++) {
            sprite = self->sprites[i];
            sprite->alpha = alpha;
            GfxSpriteSetScaleRotation(sprite, scale, scale, 0.0f, false);
        }
        if (self->comboFade <= 0.0f) {
            self->comboState = 20;
        }
        break;
    case 20:
        self->comboState = 0;
        break;
    default: /* 6..9, 12..19 and anything out of the table's range: idle */
        break;
    }

    if (self->comboState > 0) {
        count = self->comboCount;
        if (count < 0) {
            count = 0;
        }
        if (count >= 100) {
            count = 99;
        }
        if (count / 10 != 0) {
            digit = (count / 10) % 10;
            self->sprites[0]->flags |= 1;
            GfxSpriteSetCell(self->sprites[0], (float)(digit / 5), (float)(digit % 5));
        }
        digit = (count % 10) % 10;
        GfxSpriteSetCell(self->sprites[1], (float)(digit / 5), (float)(digit % 5));
    }
}
