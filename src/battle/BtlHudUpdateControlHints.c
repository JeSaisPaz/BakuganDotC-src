// bdc 0x08835df4 BtlHudUpdateControlHints
#include "bdc.h"

/* HUD widget updater (`BtlHudPhaseMain`) for the control-hint panel; does nothing unless bit 3 of
   `g_scriptGlobalBits` is set (`CoreBitsetTest`). Driven by `hintState`:
   1 resets `hintTimer`/`hintBlinkStep`/`hintFade`/`hintStick`/`hintExtraShown`, gives HUD sprites
   0x87..0x8d alpha 0, the visible bit and a zero scale/angle quad (`scaleX..angle`), hides the
   stick sprite 0x8d, clamps `hintPage` to 0..13 and builds that
   page of `g_btlHudGuidePages`: per button slot k, id -1 hides icon sprite 0x89 + k, id 0 hides
   sprite 0x89 and shows the stick sprite 0x8d (`hintStick` = 1), other ids set the icon's cell from
   `BtlHudButtonIdToCell`. Pages 8 and 10 show sprite 0xb6 (page 10 first gets the 32x32 rect,
   `BtlHudSetSpriteRect`), other pages hide it. A non-zero `extraIcon` puts sprite 0x88 at x = 373
   with cell row 0 (icon 3) or `extraIcon - 1` and sets `hintExtraShown`, else 0x88 is hidden. The
   1-4 used icons are laid out by count, the caption shown (`BtlHudShowGuideCaption`, x = 320, or
   312/328 for one icon) and the state advances to 2, falling through into it.
   2 raises `hintFade` by pi/18 (clamped to [0, pi/2]), sets the alpha of the 12 panel sprites
   (`g_btlHudHintFadeInSprites`) and, on pages 8/10, of sprite 0xb6 to sin(hintFade) and advances
   to 3 once that is >= 1.
   3 counts `hintTimer`, stepping `hintBlinkStep` every 9 frames; for extra icon 3 sprite 0x88 shows
   row 2 once the player Bakugan's `charge` (`BtlHudGetPlayerBakugan`) is >= 100, else row 0. A
   stick page cycles the stick sprite's column through `hintBlinkStep % 4`; otherwise the step wraps
   at twice the icon count and icon k shows column 1 when the extra icon is up or the step is 2k+1,
   else column 0. Pages 8/10 then centre sprite 0xb6 and pulse `hintScale` by 0.1 per frame (down
   to 0.6, then up until it reaches 4.0), scaling the sprite by `min(hintScale, 0.9)`.
   100 sets `hintFade` to pi/2 and falls into 0x65, which lowers `hintFade` by pi/18 the same way,
   sets the 12 panel sprites (`g_btlHudHintFadeOutSprites`) and sprite 0xb6 to the sine alpha and
   advances to 0x66 once it is <= 0.05. 0x66 zeroes the alpha of and hides the panel sprites
   (`g_btlHudHintHideSprites`), returns to state 0 and hides sprite 0xb6. Other states do
   nothing. */
void BtlHudUpdateControlHints(BtlHud *self)
{
    s32 panel[12];
    const BtlHudGuidePage *page;
    BtlBakugan *player;
    GfxSprite *sprite;
    s32 state;
    s32 pageIdx;
    s32 id;
    s32 count;
    s32 timer;
    s32 step;
    s32 blink;
    s32 i;
    u8 extraShown;
    float x;
    float fade;
    float value;
    float scale;

    if (CoreBitsetTest(3, g_scriptGlobalBits) == 0) {
        return;
    }
    state = self->hintState;
    switch (state) {
    case 1:
        self->hintTimer = 0;
        self->hintFade = 0.0f;
        self->hintBlinkStep = 0;
        self->hintStick = 0;
        self->hintExtraShown = 0;
        x = 320.0f;
        for (i = 0x87; i < 0x8e; i++) {
            self->sprites[i]->alpha = 0.0f;
            self->sprites[i]->flags |= 1;
            self->sprites[i]->scaleX = 0.0f;
            self->sprites[i]->scaleY = 0.0f;
            self->sprites[i]->scaleZ = 0.0f;
            self->sprites[i]->angle = 0.0f;
        }
        self->sprites[0x8d]->flags &= ~1u;
        pageIdx = self->hintPage;
        if (pageIdx < 0) {
            pageIdx = 0;
        } else if (0xd < pageIdx) {
            pageIdx = 0xd;
        }
        self->hintPage = pageIdx;
        count = 0;
        for (i = 0; i < 4; i++) {
            id = g_btlHudGuidePages[self->hintPage].buttons[i];
            sprite = self->sprites[0x89 + i];
            if (id == -1) {
                sprite->flags &= ~1u;
                continue;
            }
            if (id == 0) {
                self->sprites[0x89]->flags &= ~1u;
                self->sprites[0x8d]->flags |= 1;
                self->hintStick = 1;
            } else {
                GfxSpriteSetCell(sprite, 0.0f, (float)BtlHudButtonIdToCell(self, id));
            }
            count++;
        }
        pageIdx = self->hintPage;
        sprite = self->sprites[0xb6];
        if (pageIdx == 8 || pageIdx == 10) {
            if (pageIdx == 10) {
                BtlHudSetSpriteRect(0.0f, 0.0f, 32.0f, 32.0f, self, sprite);
                sprite = self->sprites[0xb6];
            }
            sprite->flags |= 1;
        } else {
            sprite->flags &= ~1u;
        }
        sprite = self->sprites[0x88];
        if (g_btlHudGuidePages[self->hintPage].extraIcon != 0) {
            sprite->posX = 373.0f;
            self->sprites[0x88]->maybe_billboardParams80[0] = 373.0f;
            page = &g_btlHudGuidePages[self->hintPage];
            self->hintExtraShown = 1;
            if (page->extraIcon == 3) {
                GfxSpriteSetCell(sprite, 0.0f, 0.0f);
            } else {
                GfxSpriteSetCell(sprite, 0.0f, (float)(page->extraIcon - 1));
            }
        } else {
            sprite->flags &= ~1u;
        }
        if (count < 3) {
            if (0 < count) {
                extraShown = self->hintExtraShown;
                if (count < 2) {
                    if (extraShown != 0) {
                        self->sprites[0x88]->posX = 373.0f;
                        self->sprites[0x88]->maybe_billboardParams80[0] = 373.0f;
                        self->sprites[0x89]->posX = 339.0f;
                        x = 312.0f;
                        self->sprites[0x8d]->posX = 339.0f;
                    } else {
                        self->sprites[0x89]->posX = 368.0f;
                        x = 328.0f;
                        self->sprites[0x8d]->posX = 368.0f;
                    }
                } else if (extraShown != 0) {
                    self->sprites[0x88]->posX = 356.0f;
                    self->sprites[0x88]->maybe_billboardParams80[0] = 356.0f;
                    self->sprites[0x89]->posX = 344.0f;
                    self->sprites[0x8a]->posX = 400.0f;
                    self->hintExtraShown = 0;
                } else {
                    self->sprites[0x89]->posX = 352.0f;
                    self->sprites[0x8a]->posX = 384.0f;
                }
            }
        } else if (count == 4) {
            self->sprites[0x89]->posX = 324.0f;
            self->sprites[0x8a]->posX = 352.0f;
            self->sprites[0x8b]->posX = 380.0f;
            self->sprites[0x8c]->posX = 408.0f;
        }
        BtlHudShowGuideCaption(x, self, self->hintPage);
        self->hintState = self->hintState + 1;
        /* fall through */
    case 2:
        fade = self->hintFade + 0.174532920f;
        self->hintFade = fade;
        if (fade < 0.0f) {
            fade = 0.0f;
        } else if (!(fade <= 1.57079637f)) {
            fade = 1.57079637f;
        }
        self->hintFade = fade;
        value = __builtin_sinf(fade);
        memcpy(panel, g_btlHudHintFadeInSprites, sizeof(panel));
        for (i = 0; i < 12; i++) {
            self->sprites[panel[i]]->alpha = value;
        }
        if (self->hintPage == 8 || self->hintPage == 10) {
            self->sprites[0xb6]->alpha = value;
        }
        if (!(value < 1.0f)) {
            self->hintState = self->hintState + 1;
        }
        break;
    case 3:
        timer = self->hintTimer;
        self->hintTimer = timer + 1;
        page = &g_btlHudGuidePages[self->hintPage];
        if (!(timer < 8)) {
            self->hintTimer = 0;
            self->hintBlinkStep = self->hintBlinkStep + 1;
        }
        if (page->extraIcon == 3) {
            player = (BtlBakugan *)BtlHudGetPlayerBakugan(self);
            sprite = self->sprites[0x88];
            if (!(player->charge < 100.0f)) {
                GfxSpriteSetVCell(2.0f, sprite);
            } else {
                GfxSpriteSetVCell(0.0f, sprite);
            }
        }
        step = self->hintBlinkStep;
        if (self->hintStick != 0) {
            GfxSpriteSetUCell((float)(step % 4), self->sprites[0x8d]);
        } else {
            page = &g_btlHudGuidePages[self->hintPage];
            count = 0;
            for (i = 0; i < 4; i++) {
                if (page->buttons[i] != -1) {
                    count++;
                }
            }
            if (!(step < count * 2)) {
                self->hintBlinkStep = 0;
            }
            blink = 1;
            for (i = 0; i < 4; i++) {
                sprite = self->sprites[0x89 + i];
                if (self->hintExtraShown != 0) {
                    GfxSpriteSetUCell(1.0f, sprite);
                } else if (self->hintBlinkStep == blink) {
                    GfxSpriteSetUCell(1.0f, sprite);
                } else {
                    GfxSpriteSetUCell(0.0f, sprite);
                }
                blink += 2;
            }
        }
        pageIdx = self->hintPage;
        if (pageIdx != 8 && pageIdx != 10) {
            break;
        }
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
        break;
    case 100:
        self->hintFade = 1.57079637f;
        self->hintState = state + 1;
        /* fall through */
    case 0x65:
        fade = self->hintFade - 0.174532920f;
        self->hintFade = fade;
        if (fade < 0.0f) {
            fade = 0.0f;
        } else if (!(fade <= 1.57079637f)) {
            fade = 1.57079637f;
        }
        self->hintFade = fade;
        value = __builtin_sinf(fade);
        memcpy(panel, g_btlHudHintFadeOutSprites, sizeof(panel));
        for (i = 0; i < 12; i++) {
            self->sprites[panel[i]]->alpha = value;
        }
        self->sprites[0xb6]->alpha = value;
        if (value <= 0.0500000007f) {
            self->hintState = self->hintState + 1;
        }
        break;
    case 0x66:
        memcpy(panel, g_btlHudHintHideSprites, sizeof(panel));
        for (i = 0; i < 12; i++) {
            sprite = self->sprites[panel[i]];
            sprite->alpha = 0.0f;
            sprite->flags &= ~1u;
        }
        self->hintState = 0;
        self->sprites[0xb6]->flags &= ~1u;
        break;
    default:
        break;
    }
}
