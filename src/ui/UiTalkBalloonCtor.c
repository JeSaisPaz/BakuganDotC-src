// bdc 0x088c9370 UiTalkBalloonCtor
#include "bdc.h"

/* Constructor of the talk balloon task (`UiTalkBalloon`, speech-bubble message window of field
   events: 0x220 bytes, created by `UiTalkBalloonCreate`): `CoreTaskInit`, vtable
   `g_uiTalkBalloonVtbl` (slot 1 `UiTalkBalloonDtor`, 2 `UiTalkBalloonUpdate`, 4
   `UiTalkBalloonDraw`), clears the text/layout state, sets default sizes (glyph width 15, line
   height 20, reveal speed 1.5), the highlight colour (black, alpha 0.6), the depth
   (10 + `g_uiTalkBalloonDepthBias`, which then grows by 0.01) and the line count
   `g_uiTalkBalloonLineCount`, then creates the 2D sprite layer `partLayer` (`+0x208`) and its 9
   part sprites `parts` (`+0x20c`, layout 0x13, `UiLayoutCreateSprites`) with their visible bit
   cleared. Returns `self`.
   `origin` and `highlights[7]` are zeroed (the VFPU bank's zero vector C720). */

UiTalkBalloon *UiTalkBalloonCtor(UiTalkBalloon *self)
{
    GfxSpriteLayer *layer;
    GfxSpriteLayer *mem;
    GfxSprite **parts;
    bool fromLow;
    s32 i;

    CoreTaskInit(&self->base);
    self->base.vtable = g_uiTalkBalloonVtbl;
    self->text = NULL;
    self->textCursor = NULL;
    self->holdOpen = 0;
    self->origin[0] = 0.0f; /* sv.q of the bank's zero vector C720 */
    self->origin[1] = 0.0f;
    self->origin[2] = 0.0f;
    self->origin[3] = 0.0f;
    self->resetWord40 = 0;
    self->resetWord44 = 0;
    self->state = 0;
    self->revealSpeed = 1.5f;
    self->revealTimer = 0.0f;
    self->stateTimer = 0;
    self->frameSprite = NULL;
    self->iconSprite = NULL;
    self->printer = NULL;
    self->pageFlags[0] = 0;
    self->pageFlags[1] = 0;
    self->glyphWidth = 15.0f;
    self->lineHeight = 20.0f;
    self->highlightTexture = NULL;
    self->highlightCount = 0;
    self->pageFlags[2] = 0;
    self->highlightColor[0] = g_colorBlack.x;
    self->highlightColor[1] = g_colorBlack.y;
    self->highlightColor[2] = g_colorBlack.z;
    self->highlightColor[3] = g_colorBlack.w;
    self->highlightColor[3] = 0.6f;
    self->lineHighlighted = 0;
    self->headerLine = 0;
    self->inInsert = 0;
    self->insertText = NULL;
    self->hasHeader = 0;
    self->resetWord128 = 0;
    self->resetWord15c = 0;
    self->resetWord170 = 0;
    self->lineIndent = 0.0f;
    self->autoAdvance = 0;
    self->choiceTop = 0.0f;
    self->choiceIndex = 0;
    self->choiceCount = 0;
    self->choiceMode = 0;
    self->cursorSprite = NULL;
    self->glyphSerial = 0;
    self->holdFrames = 0;
    self->choiceFrame = 0;
    self->buffer = NULL;
    self->frameHidden = 0;
    self->tailSidesLocked = 0;
    self->tailSides = 0;
    self->depth = g_uiTalkBalloonDepthBias + 10.0f;
    g_uiTalkBalloonDepthBias = g_uiTalkBalloonDepthBias + 0.01f;
    self->lineCount = g_uiTalkBalloonLineCount;
    self->lineIndex = 0;
    self->voiceTable = NULL;
    self->resetWord1fc = 0;
    self->highlights[7][0] = 0.0f; /* sv.q of the bank's zero vector C720 */
    self->highlights[7][1] = 0.0f;
    self->highlights[7][2] = 0.0f;
    self->highlights[7][3] = 0.0f;
    self->speaker = 0;
    self->portraitFrame = 0;
    self->speakerName = NULL;

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
    self->partLayer = layer;
    layer->sorted = 1;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    parts = MemAlloc(0x24, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->parts = parts;
    UiLayoutCreateSprites(self->partLayer, parts, 0x13);
    for (i = 0; i < 9; i++) {
        self->parts[i]->flags &= ~1u;
    }
    return self;
}
