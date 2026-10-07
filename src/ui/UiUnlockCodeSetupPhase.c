// bdc 0x08994070 UiUnlockCodeSetupPhase
#include "bdc.h"

/* Phase 1 of `UiUnlockCode`: creates the 0x3e layout sprites
   (`UiLayoutCreateSprites`, array of 0x29 sprite pointers (0xa4 bytes) at `data`), two text
   printers (`keyText`, font 3, scale 0.9, wrap 32; `digitText`, font 3, scale 0.7, wrap 20; left
   NULL if the allocation fails), clones the helper sprites (OK highlight slot 32, key-pop slot 33,
   caret slot 34, six black key shadows in slots 35..40) with `UiUnlockCodeCloneSprite`, sets
   colours (`g_unlockCodeColorDarkGreen`, `g_unlockCodeColorDimGreen`,
   `g_unlockCodeColorGreen`, `g_unlockCodeColorLightGreen`, `g_colorBlack`), layer masks,
   depths, positions and cells, hides sprites 1..40 (alpha 0, flags bit0 cleared) and goes to
   phase 2. */

#define SPR(i) (((GfxSprite **)self->base.data)[i])

/* Copies an RGBA colour into the sprite's tint/alpha quad (one lv.q/sv.q pair in the binary). */
static inline void UiUnlockCodeSetSpriteColor(GfxSprite *sprite, const float *rgba)
{
    sprite->tint[0] = rgba[0];
    sprite->tint[1] = rgba[1];
    sprite->tint[2] = rgba[2];
    sprite->alpha = rgba[3];
}

static inline UiTextPrinter *UiUnlockCodeNewPrinter(void)
{
    bool fromLow;
    UiTextPrinter *printer;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    printer = MemAlloc(0xf0, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (printer != NULL) {
        UiTextPrinterCtor(printer, NULL, NULL);
    }
    return printer;
}

void UiUnlockCodeSetupPhase(UiUnlockCode *self)
{
    bool fromLow;
    GfxSprite **sprites;
    GfxSprite *sprite;
    float w;
    float h;
    float z;
    float uvRect[4];
    int i;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    sprites = MemAlloc(0xa4, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->base.data = sprites;
    UiLayoutCreateSprites(self->base.spriteLayer, sprites, 0x3e);

    self->keyText = UiUnlockCodeNewPrinter();
    UiTextPrinterSetFont(self->keyText, 3);
    self->keyText->scale = 0.9f;
    self->keyText->wrapWidth = 32.0f;

    self->digitText = UiUnlockCodeNewPrinter();
    UiTextPrinterSetFont(self->digitText, 3);
    self->digitText->scale = 0.7f;
    self->digitText->wrapWidth = 20.0f;

    /* OK highlight: clone of sprite 27 */
    GfxSpriteCenterPivot(SPR(27));
    GfxSpriteSetScaleRotation(SPR(27), 1.0f, 1.0f, 0.0f, false);
    SPR(32) = UiUnlockCodeCloneSprite(SPR(27), self->base.spriteLayer);
    sprite = SPR(32);
    w = GfxSpriteGetWidth(SPR(27));
    h = GfxSpriteGetHeight(SPR(27));
    UiSpriteSetSize(w, h, sprite);

    /* key-pop: clone of key sprite 10 */
    SPR(33) = UiUnlockCodeCloneSprite(SPR(10), self->base.spriteLayer);
    sprite = SPR(33);
    w = GfxSpriteGetWidth(SPR(10));
    h = GfxSpriteGetHeight(SPR(10));
    UiSpriteSetSize(w, h, sprite);

    /* caret: clone of sprite 2, 32x32 cell at (0, 8) */
    SPR(34) = UiUnlockCodeCloneSprite(SPR(2), self->base.spriteLayer);
    uvRect[0] = 0.0f;
    uvRect[1] = 8.0f;
    uvRect[2] = 32.0f;
    uvRect[3] = 32.0f;
    GfxSpriteSetUvRectXYWH(SPR(34), uvRect);
    UiSpriteSetSize(32.0f, 32.0f, SPR(34));

    SPR(20)->posZ -= 1.0f;
    SPR(21)->posZ -= 1.0f;

    /* six key shadows: black clones of sprites 4..9, one unit behind and offset by (1, 1) */
    for (i = 0; i < 6; i++) {
        SPR(4 + i)->posZ -= 1.0f;
        SPR(35 + i) = UiUnlockCodeCloneSprite(SPR(4 + i), self->base.spriteLayer);
        UiUnlockCodeSetSpriteColor(SPR(35 + i), &g_colorBlack.x);
        SPR(35 + i)->posZ += 1.0f;
        SPR(35 + i)->posX += 1.0f;
        SPR(35 + i)->posY += 1.0f;
    }

    SPR(0)->layerMask = 2;
    SPR(11)->layerMask = 4;
    SPR(20)->layerMask = 4;
    SPR(21)->layerMask = 4;
    SPR(26)->layerMask = 4;

    sprites = (GfxSprite **)self->base.data;
    UiUnlockCodeSetSpriteColor(sprites[23], &g_unlockCodeColorDarkGreen.x);
    UiUnlockCodeSetSpriteColor(sprites[15], sprites[23]->tint);
    UiUnlockCodeSetSpriteColor(sprites[14], sprites[15]->tint);
    UiUnlockCodeSetSpriteColor(sprites[2], sprites[14]->tint);

    sprites = (GfxSprite **)self->base.data;
    UiUnlockCodeSetSpriteColor(sprites[25], &g_unlockCodeColorDimGreen.x);
    UiUnlockCodeSetSpriteColor(sprites[24], sprites[25]->tint);

    UiUnlockCodeSetSpriteColor(SPR(3), &g_unlockCodeColorGreen.x);
    UiUnlockCodeSetSpriteColor(SPR(4), &g_unlockCodeColorLightGreen.x);
    UiUnlockCodeSetSpriteColor(SPR(34), &g_unlockCodeColorDimGreen.x);

    SPR(13)->alpha = 0.5f;
    SPR(13)->textureSlot = 1;
    SPR(12)->alpha = 0.5f;

    SPR(26)->posX = 104.5f;
    SPR(26)->posY = 33.5f;
    SPR(26)->posZ = SPR(18)->posZ - 1.0f;

    z = SPR(25)->posZ;
    SPR(24)->posZ = z;
    SPR(23)->posZ = z;
    SPR(15)->posZ = z;
    SPR(14)->posZ = z;

    z = SPR(25)->posZ - 1.0f;
    SPR(21)->posZ = z;
    SPR(20)->posZ = z;

    sprite = SPR(11);
    z = SPR(3)->posZ - 1.0f;
    sprite->posX = 91.0f;
    sprite->posY = 63.5f;
    sprite->posZ = z;
    sprite->posW = 0.0f;

    SPR(20)->posZ -= 1.0f;
    SPR(21)->posZ -= 1.0f;

    GfxSpriteCenterPivot(SPR(10));
    GfxSpriteResetMatrix(SPR(10));
    GfxSpriteCenterPivot(SPR(16));
    GfxSpriteResetMatrix(SPR(16));
    GfxSpriteCenterPivot(SPR(17));
    GfxSpriteResetMatrix(SPR(17));
    GfxSpriteSetVCell(2.0f, SPR(28));
    GfxSpriteSetVCell(1.0f, SPR(29));

    sprite = SPR(34);
    z = SPR(2)->posZ - 0.5f;
    sprite->posX = 93.0f;
    sprite->posY = 142.0f;
    sprite->posZ = z;
    sprite->posW = 0.0f;

    for (i = 1; i < 0x29; i++) {
        SPR(i)->alpha = 0.0f;
        SPR(i)->flags &= ~1u;
    }

    self->base.phase = 2;
}
