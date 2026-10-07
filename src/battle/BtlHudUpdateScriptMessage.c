// bdc 0x088450bc BtlHudUpdateScriptMessage
#include "bdc.h"

/* HUD widget (`BtlHudPhaseMain`) for the script-triggered battle message popup, a step machine on
   `msgStep`: `UiTalkRequestClose` (from `ScriptOpTalkMessage` cmd 4) clears UI window 0xb,
   stores the message index in `msgIndex` and sets step 1. Nothing runs while window 0xb is
   active, while the battle task (`BtlMain.busy`) is busy, or before script flag 0x22 is set.
   Step 1 aborts (999) unless script global 8 (battle rule mode) is 1; 2 builds the 2-sprite
   layout 8 in `msgLayer` (hidden, sprite 0 moved 1 towards the camera) and shows HUD sprites
   0xf0 (panel) and 0xef (prompt icon at 436,144); 3/4 wait 15 frames (`msgWait`); 10 prints
   `msgTable[msgIndex]` into `overlayObj[1]` centred in a 400-px box (font 1, `UiTextMeasure`),
   plays its voice (`g_btlScriptMsgVoiceIds`, default 0x2a7b, `SndBgmPlayVoice`); 10/11 fade
   the panel in (alpha `sin(msgFade)`, +5 deg per frame, layout sprites centred on x 240) and go
   to 100 at full alpha. 99 builds and prints everything at once at full alpha. 100 animates the
   prompt icon cell (`GfxSpriteSetUCell`, 16-frame cycle in `posW`) until cross (pressed bit
   0x4000) is pressed; 101/102 fade out (-10 deg per frame), then 999 hides both HUD sprites,
   clears the text layer, resets the step to 0 and re-activates window 0xb. 20/21 fade layout
   sprite 1 in and also end at 100. Other steps do nothing.
   The fade sine is `vsin.s` of angle * S703 (2/pi), i.e. sin(angle); the panel move is a `vadd.t`
   of (0, 0, -1) on sprite 0's position. */

#define SCRIPT_MSG_HALF_PI   1.57079637f    /* 0x3fc90fdb */
#define SCRIPT_MSG_FADE_IN   0.0872664601f  /* 0x3db2b8c2, 5 degrees */
#define SCRIPT_MSG_FADE_OUT  -0.17453292f   /* 0xbe32b8c2, -10 degrees */
#define SCRIPT_MSG_PANEL_A   0.600000024f   /* 0x3f19999a */

/* sin(angle): `vsin.s` of angle * 2/pi (quarter turns) on the VFPU. */
static inline float ScriptMsgVfpuSin(float angle)
{
    return __builtin_sinf(angle);
}

/* Creates `msgLayer` / `msgSprites` if missing, builds layout 8, sets the layout sprites' alpha to
   `alpha`, moves sprite 0 by (0, 0, -1) and shows HUD sprites 0xf0 (alpha
   `panelAlpha`) and 0xef (at 436,144, cell counter 0, alpha `alpha`). */
static void ScriptMsgBuildPanel(BtlHud *self, float alpha, float panelAlpha)
{
    GfxSprite *first;
    GfxSpriteLayer *layer;
    GfxSpriteLayer *mem;
    GfxSprite **sprites;
    bool fromLow;
    s32 i;

    if (self->msgLayer == NULL) {
        layer = NULL;
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        mem = MemAlloc(0x80, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (mem != NULL) {
            GfxSpriteLayerCtor(mem, 0);
            layer = mem;
        }
        self->msgLayer = layer;
    }
    self->msgLayer->sorted = 1;
    if (self->msgSprites == NULL) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        sprites = MemAlloc(8, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        self->msgSprites = sprites;
    }
    UiLayoutCreateSprites(self->msgLayer, self->msgSprites, 8);
    for (i = 0; i < 2; i++) {
        self->msgSprites[i]->alpha = alpha;
    }
    /* vadd.t of (0, 0, -1) onto posX/posY/posZ (posW is stored back unchanged) */
    first = self->msgSprites[0];
    first->posX = first->posX + 0.0f;
    first->posY = first->posY + 0.0f;
    first->posZ = first->posZ + -1.0f;
    self->sprites[0xf0]->flags |= 1;
    self->sprites[0xf0]->alpha = panelAlpha;
    self->sprites[0xef]->flags |= 1;
    self->sprites[0xef]->posX = 436.0f;
    self->sprites[0xef]->posY = 144.0f;
    self->sprites[0xef]->posW = 0.0f;
    self->sprites[0xef]->alpha = alpha;
}

/* Copies `msgTable[msgIndex]` into `msgText` and prints it with the text printer `overlayObj[1]`
   (cleared first; wrap width 400, font 1, width scale 0.6), centred horizontally in the 400-px box
   from x 40 and at y 104, or y 112 when the text is lower than 17. */
static void ScriptMsgPrint(BtlHud *self)
{
    UiTextPrinter *printer;
    const VtblEntry *print;
    s32 x;
    s32 y;
    s32 offset;

    y = 0x68;
    x = 0x28;
    strcpy(self->msgText, self->msgTable[self->msgIndex]);
    printer = self->overlayObj[1];
    GfxSpriteLayerClear(&printer->layer);
    printer->glyphs = NULL;
    printer = self->overlayObj[1];
    printer->wrapWidth = 400.0f;
    UiTextPrinterSetFont(self->overlayObj[1], 1);
    printer = self->overlayObj[1];
    printer->widthScale = SCRIPT_MSG_PANEL_A;
    /* The listing passes +0x8c8 as the 4th and +0x8c4 as the 5th argument. */
    UiTextMeasure(0.0f, self->overlayObj[1], self->msgText, &self->talkTextHeight,
                  &self->talkTextWidth, NULL);
    offset = (s32)((400.0f - self->talkTextWidth) * 0.5f);
    if (offset >= 0) {
        x = offset + 0x28;
    }
    if (self->talkTextHeight < 17.0f) {
        y = 0x70;
    }
    printer = self->overlayObj[1];
    print = &printer->layer.vtbl[2];
    ((void (*)(float, float, float, void *, char *, s32, s32, s32))print->fn)(
        (float)x, (float)y, 0.0f, (u8 *)printer + print->delta, self->msgText, 0, 0, 0);
}

/* Hides HUD sprites 0xef/0xf0, clears the text printer, resets the step and re-activates UI
   window 0xb. */
static void ScriptMsgClose(BtlHud *self)
{
    UiTextPrinter *printer;

    self->sprites[0xef]->flags &= ~1u;
    self->sprites[0xf0]->flags &= ~1u;
    printer = self->overlayObj[1];
    GfxSpriteLayerClear(&printer->layer);
    printer->glyphs = NULL;
    self->msgStep = 0;
    UiSetWindowActive(0xb, 1);
}

void BtlHudUpdateScriptMessage(BtlHud *self)
{
    GfxSprite *sprite;
    GfxSprite *icon;
    s32 step;
    s32 voiceId;
    s32 i;
    float angle;
    float alpha;
    float width;

    if (UiGetWindowActive(0xb) == 1) {
        return;
    }
    if (BtlCameraTaskExists() != 0 && ((BtlMain *)BtlGetCameraTask())->busy != 0) {
        return;
    }
    if (!CoreBitsetTest(0x22, g_scriptGlobalBits)) {
        return;
    }

    step = self->msgStep;
    if (step >= 0x65) {
        if (step >= 0x67) {
            if (step == 999) {
                ScriptMsgClose(self);
            }
            return;
        }
        if (step < 0x66) {
            /* 101: start the fade-out */
            self->msgWait = 0;
            self->msgFade = SCRIPT_MSG_HALF_PI;
            self->msgStep++;
        }
        /* 102: fade out */
        angle = self->msgFade + SCRIPT_MSG_FADE_OUT;
        self->msgFade = angle;
        if (angle < 0.0f) {
            angle = 0.0f;
        } else if (!(angle <= SCRIPT_MSG_HALF_PI)) {
            angle = SCRIPT_MSG_HALF_PI;
        }
        self->msgFade = angle;
        alpha = ScriptMsgVfpuSin(angle);
        for (i = 0; i < 2; i++) {
            self->msgSprites[i]->alpha = alpha;
        }
        self->talkTextAlpha = alpha;
        self->sprites[0xef]->alpha = alpha;
        self->sprites[0xf0]->alpha = alpha * SCRIPT_MSG_PANEL_A;
        if (alpha <= 0.0f) {
            self->msgStep = 999;
            ScriptMsgClose(self);
        }
        return;
    }

    if (step >= 0x16) {
        if (step < 99) {
            return;
        }
        if (step < 100) {
            /* 99: build and print at full alpha, then continue as 100 */
            ScriptMsgBuildPanel(self, 1.0f, SCRIPT_MSG_PANEL_A);
            ScriptMsgPrint(self);
            self->talkTextAlpha = 1.0f;
            self->msgStep++;
        }
        /* 100: wait for cross, animating the prompt icon */
        if ((self->pad->pressed & 0x4000) != 0) {
            self->msgStep++;
            return;
        }
        self->sprites[0xef]->posW += 1.0f;
        if (!(self->sprites[0xef]->posW < 16.0f)) {
            self->sprites[0xef]->posW = 0.0f;
        }
        icon = self->sprites[0xef];
        GfxSpriteSetUCell((float)(s32)(icon->posW * 0.125f), icon);
        return;
    }

    switch (step) {
    case 1:
        if (g_scriptGlobalVars[8] != 1) {
            self->msgStep = 999;
            return;
        }
        self->msgStep++;
        return;
    case 2:
        ScriptMsgBuildPanel(self, 0.0f, 0.0f);
        self->msgStep++;
        /* fall through */
    case 3:
        self->msgWait = 15;
        self->msgStep++;
        /* fall through */
    case 4:
        self->msgWait--;
        if (self->msgWait > 0) {
            return;
        }
        self->msgStep = 10;
        return;
    case 10:
        ScriptMsgPrint(self);
        self->talkTextAlpha = 0.0f;
        self->msgFade = 0.0f;
        self->msgStep++;
        voiceId = 0x2a7b;
        if (g_btlScriptMsgVoiceIds[self->msgIndex - 0x5e] != -1) {
            voiceId = g_btlScriptMsgVoiceIds[self->msgIndex - 0x5e];
        }
        SndBgmPlayVoice(voiceId);
        /* fall through */
    case 11:
        /* fade in */
        angle = self->msgFade + SCRIPT_MSG_FADE_IN;
        self->msgFade = angle;
        if (angle < 0.0f) {
            self->msgFade = 0.0f;
            angle = 0.0f;
        } else {
            if (!(angle <= SCRIPT_MSG_HALF_PI)) {
                angle = SCRIPT_MSG_HALF_PI;
            }
            self->msgFade = angle;
        }
        alpha = ScriptMsgVfpuSin(angle);
        self->talkTextAlpha = alpha;
        for (i = 0; i < 2; i++) {
            sprite = self->msgSprites[i];
            sprite->alpha = alpha;
            width = GfxSpriteGetWidth(sprite);
            sprite->posX = (float)(0xf0 - (s32)width / 2);
        }
        self->sprites[0xf0]->alpha = alpha * SCRIPT_MSG_PANEL_A;
        self->sprites[0xef]->alpha = alpha;
        if (!(alpha < 1.0f)) {
            self->msgStep = 100;
        }
        return;
    case 20:
        self->msgFade = 0.0f;
        self->msgStep++;
        /* fall through */
    case 21:
        /* fade in layout sprite 1 only */
        angle = self->msgFade + SCRIPT_MSG_FADE_IN;
        self->msgFade = angle;
        if (angle < 0.0f) {
            angle = 0.0f;
        } else if (!(angle <= SCRIPT_MSG_HALF_PI)) {
            angle = SCRIPT_MSG_HALF_PI;
        }
        self->msgFade = angle;
        alpha = ScriptMsgVfpuSin(angle);
        self->msgSprites[1]->alpha = alpha;
        if (!(alpha < 1.0f)) {
            self->msgStep = 100;
        }
        return;
    default:
        /* 0, 5..9, 12..19 and negative steps: idle */
        return;
    }
}
