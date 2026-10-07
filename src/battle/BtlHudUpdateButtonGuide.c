// bdc 0x08836848 BtlHudUpdateButtonGuide
#include "bdc.h"

/* HUD widget updater (`BtlHudPhaseMain`) for the button-guide panel, driven by `guideState` and
   `guidePage`. Nothing happens in state 0. With advisor comments off in the save profile
   (`adviceOff == 1`) a guide in state 3 is first dismissed (`BtlHudRequestButtonGuide`).
   State 1 hides HUD sprites 0x87..0x8d (alpha 0, visible bit cleared, scale/angle quad
   `scaleX..angle` zeroed from the VFPU bank's C720 = 0), shows sprite 0x87 and, for each
   used button id of the page in `g_btlHudGuidePages`, shows icon sprite 0x89 + k at x = 368 with
   the cell from `BtlHudButtonIdToCell`; then shows the caption (`BtlHudShowGuideCaption` at
   x = 320), resets `guideFade`, advances to state 2 and, on page 8, shows sprite 0xb6. State 2 raises
   `guideFade` by π/18 (clamped to [0, π/2]) and sets the alpha of the 7 panel sprites (0x87, 0x89,
   0x9e, 0xf7..0xfa) and of sprite 0xb6 to sin(guideFade) (`vsin` of guideFade times the bank's
   S703 = 2/π), advancing to state 3 once that is >= 1. State 3 holds until `guideShow` clears (then
   state 4); on page 8 it centres sprite 0xb6 and pulses `hintScale` by 0.1 per frame (down to 0.6,
   then up until it reaches 4.0), scaling the sprite by `min(hintScale, 0.9)`. State 4 lowers
   `guideFade` the same way, sets the same alphas, and returns to state 0 when the sine is < 0. */
void BtlHudUpdateButtonGuide(BtlHud *self)
{
    s32 panel[7];
    const BtlHudGuidePage *page;
    GfxSprite *sprite;
    s32 state;
    s32 i;
    float fade;
    float value;
    float scale;

    if (self->guideState == 0) {
        return;
    }
    state = self->guideState;
    if (((SaveProfile *)SaveGetProfile())->data->adviceOff == 1 && state == 3) {
        BtlHudRequestButtonGuide(self, false, 0);
        state = self->guideState;
    }
    panel[0] = 0x87;
    panel[1] = 0x89;
    panel[2] = 0x9e;
    panel[3] = 0xf7;
    panel[4] = 0xf8;
    panel[5] = 0xf9;
    panel[6] = 0xfa;

    if (state == 1) {
        for (i = 0x87; i < 0x8e; i++) {
            self->sprites[i]->alpha = 0.0f;
            self->sprites[i]->flags &= ~1u;
            /* sv.q of the bank's C720 = (0, 0, 0, 0) */
            self->sprites[i]->scaleX = 0.0f;
            self->sprites[i]->scaleY = 0.0f;
            self->sprites[i]->scaleZ = 0.0f;
            self->sprites[i]->angle = 0.0f;
        }
        self->sprites[0x87]->flags |= 1;
        for (i = 0; i < 4; i++) {
            if (g_btlHudGuidePages[self->guidePage].buttons[i] != -1) {
                sprite = self->sprites[0x89 + i];
                sprite->flags |= 1;
                page = &g_btlHudGuidePages[self->guidePage];
                GfxSpriteSetCell(sprite, 0.0f, (float)BtlHudButtonIdToCell(self, page->buttons[i]));
                sprite->posX = 368.0f;
            }
        }
        BtlHudShowGuideCaption(320.0f, self, self->guidePage);
        self->guideFade = 0.0f;
        self->guideState = self->guideState + 1;
        if (self->guidePage == 8) {
            self->sprites[0xb6]->flags |= 1;
        }
    } else if (state == 2) {
        fade = self->guideFade + 0.174532920f;
        if (fade < 0.0f) {
            fade = 0.0f;
        } else if (!(fade <= 1.57079637f)) {
            fade = 1.57079637f;
        }
        self->guideFade = fade;
        value = __builtin_sinf(fade);
        for (i = 0; i < 7; i++) {
            self->sprites[panel[i]]->alpha = value;
        }
        self->sprites[0xb6]->alpha = value;
        if (!(value < 1.0f)) {
            self->guideState = self->guideState + 1;
        }
    } else if (state == 3) {
        if (self->guideShow == 0) {
            self->guideState = state + 1;
        } else if (self->guidePage == 8) {
            GfxSpriteCenterPivot(self->sprites[0xb6]);
            scale = self->hintScale;
            if (self->hintScaleGrowing != 0) {
                scale = scale + 0.100000001f;
                self->hintScale = scale;
                if (!(scale < 4.0f)) {
                    self->hintScaleGrowing = 0;
                }
            } else {
                scale = scale - 0.100000001f;
                self->hintScale = scale;
                if (scale <= 0.600000024f) {
                    scale = 0.600000024f;
                    self->hintScale = scale;
                    self->hintScaleGrowing = 1;
                }
            }
            if (!(scale < 0.899999976f)) {
                scale = 0.899999976f;
            }
            GfxSpriteSetScaleRotation(self->sprites[0xb6], scale, scale, 0.0f, false);
        }
    } else if (state == 4) {
        fade = self->guideFade - 0.174532920f;
        if (fade < 0.0f) {
            fade = 0.0f;
        } else if (!(fade <= 1.57079637f)) {
            fade = 1.57079637f;
        }
        self->guideFade = fade;
        value = __builtin_sinf(fade);
        for (i = 0; i < 7; i++) {
            self->sprites[panel[i]]->alpha = value;
        }
        if (value < 0.0f) {
            self->guideState = 0;
        }
        self->sprites[0xb6]->alpha = value;
    }
}
