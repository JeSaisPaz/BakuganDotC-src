// bdc 0x0883b770 BtlHudPhaseBuild
#include "bdc.h"

/* HUD phase 1 handler (`BtlHudUpdate` phase table `0x08a64b9c`): builds the whole battle HUD.
   Allocates the sprite layer (0x106 pooled sprites) and the sprite array, creates UI layout 4
   (`UiLayoutCreateSprites`) hidden, then sets up for the player Bakugan (`BtlGetPlayerBakugan`)
   the sphere icon (`"baku_sphere_%03d"`, ids 1..20), its attribute icon (virtual slot 20,
   `BtlHudSetAttributeIcon`) and the six status icons (`iconTarget`), the name plates of the
   other Bakugan (`"game_baku_name"`), the item icons (`"game_item_%02d"`), the enemy arrows and
   their palette blenders (`arrowShownPos`/`arrowHiddenPos`), the timer scroll bars, the lock-on
   marker blenders, the combo digits (`comboDigitPos`), the `"hisan_eff"`/`"housha_hikari"`
   effect sprites, the charge gauges (`"card_ura"` fill sprites at `gaugeAnchor`, glow, frame and
   surface sprites) with the art cards (`"card_L_%03d.lzs"` unpacked with `CoreLzssDecompress` into `buildPixels`,
   texture `buildTex`), the score board in score mode (script global 8 == 2), the radar (16 blips
   `"ga_rad_01"`..`"ga_rad_07"`), the ability cut-in, guard and finish sprites, the start banner
   (`"fab_start01"`..`04` CLUTs recoloured with the attribute hue `g_btlHudAttributeHues`), the
   four `.fab` objects (start, gauge, gauge-one, gauge-loop; `BtlHudCreateTextObject`), the two
   talk text printers (`overlayObj`), the battle message table (`"DRMesBattleCommon"`), the
   round-count banner (profile words 0x1b/0x1e) and the round-win lamps from the battle main
   task's `roundResults` (win/loss swapped for a net guest). Then moves on to the next phase
   (`phase + 1`, `phaseStep` 0).
   Bank constants: the `sv.q C720` stores are zeros (gaugeAnchor[0..2], talkTextPos) and S701 is
   the 255 that scales the clamped RGBA before packing the recoloured CLUT entries. The quad copies
   (`lv.q`/`sv.q` through C000) are plain 4-float copies; the `vrndi.s` scale is PlatformRandU32. */

/* Copies the 4 floats at `src` to `dst`. */
static inline void HudCopyVec4(float *dst, const float *src)
{
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}

/* Zeroes the 4 floats at `dst` (the bank vector C720). */
static inline void HudZeroVec4(float *dst)
{
    dst[0] = 0.0f;
    dst[1] = 0.0f;
    dst[2] = 0.0f;
    dst[3] = 0.0f;
}

/* Copies a sprite's position quad (posX..posW) to `dst`. */
static inline void HudGetPos(float *dst, const GfxSprite *sprite)
{
    dst[0] = sprite->posX;
    dst[1] = sprite->posY;
    dst[2] = sprite->posZ;
    dst[3] = sprite->posW;
}

/* Sets a sprite's position quad (posX..posW) from `src`. */
static inline void HudSetPos(GfxSprite *sprite, const float *src)
{
    sprite->posX = src[0];
    sprite->posY = src[1];
    sprite->posZ = src[2];
    sprite->posW = src[3];
}

/* Copies the position quad of sprite `src` to sprite `dst`. */
static inline void HudCopyPos(GfxSprite *dst, const GfxSprite *src)
{
    dst->posX = src->posX;
    dst->posY = src->posY;
    dst->posZ = src->posZ;
    dst->posW = src->posW;
}

/* Sets a sprite's colour quad (tint RGB + alpha) from `rgba`. */
static inline void HudSetColor(GfxSprite *sprite, const float *rgba)
{
    sprite->tint[0] = rgba[0];
    sprite->tint[1] = rgba[1];
    sprite->tint[2] = rgba[2];
    sprite->alpha = rgba[3];
}

/* Allocates `size` bytes from the low end of the heap, restoring the previous side. */
static inline void *HudAllocLow(s32 size)
{
    bool fromLow;
    void *mem;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(size, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    return mem;
}

/* The unit's attribute: virtual slot 20 (`+0xa0`) of its vtable. */
static inline int HudUnitAttribute(BtlBakugan *unit)
{
    const VtblEntry *entry = &((const VtblEntry *)unit->base.base.vtable)[20];

    return ((int (*)(void *))entry->fn)((u8 *)unit + entry->delta);
}

/* Art id of combat slot `slot`; slots past 2 read slot 0. */
static inline s32 HudArtId(const BtlBakugan *unit, u8 slot)
{
    return slot < 3 ? unit->combat.artIds[slot] : unit->combat.artIds[0];
}

/* vrndi.s: 32 random bits from the VFPU generator. */
static inline u32 HudRandomBits(void)
{
    return PlatformRandU32();
}

/* One RGBA8 CLUT entry, written as a word and then its alpha byte. */
typedef union {
    u32 word;
    u8 rgba[4];
} HudClutEntry;

/* GfxSprite.flags bit0 is "visible". */
#define HUD_SPRITE_VISIBLE 1u

void BtlHudPhaseBuild(BtlHud *self)
{
    BtlBakugan *unit;
    BtlBakugan *other;
    BtlMain *battle;
    GfxSpriteLayer *layer;
    GfxSprite *sprite;
    GfxSprite *sprite2;
    GfxSprite *glow;
    GfxSprite *art;
    GfxSprite *gaugeFill;
    GfxSprite *frame;
    GfxSprite *surface;
    GfxFab *fab;
    UiTextPrinter *printer;
    void *mem;
    void *obj;
    void *texture;
    void *blob;
    u8 *pixels;
    HudClutEntry *clut;
    u32 packed;
    u32 bits;
    s32 rounds;
    s32 roundIndex;
    s32 word;
    s32 players;
    s32 targets;
    s32 attribute;
    s32 result;
    s32 wins;
    s32 losses;
    s32 msgId;
    s32 colorCount;
    s32 i;
    s32 j;
    u8 alpha;
    u8 multiPlayer;
    float drop;
    float yOffset;
    float a;
    float cell;
    const float *hue;
    char name[0x20];
    s32 shownSprites[7];
    s32 slotWord[4];
    s32 nameCell[3];
    s32 shiftSprites[5];
    const char *startTextures[4];
    float tint[4];
    float pos[4];
    float uv[4];
    float rgba[4];

    unit = (BtlBakugan *)BtlGetPlayerBakugan();

    /* round count (profile word 0x1b, 1..5) and current round (word 0x1e, 0..4) */
    word = (s32)SaveProfileGetWord(SaveGetProfile(), 0x1b);
    if (word < 1) {
        rounds = 1;
    } else if (5 < word) {
        rounds = 5;
    } else {
        rounds = word;
    }
    word = (s32)SaveProfileGetWord(SaveGetProfile(), 0x1e);
    if (word < 0) {
        roundIndex = 0;
    } else if (4 < word) {
        roundIndex = 4;
    } else {
        roundIndex = word;
    }
    multiPlayer = 0;
    if (!(BtlCountPlayerBakugan() < 2)) {
        multiPlayer = 1;
    }

    /* sprite layer and the layout 4 sprites, all hidden */
    mem = HudAllocLow(sizeof(GfxSpriteLayer));
    layer = NULL;
    if (mem != NULL) {
        GfxSpriteLayerCtor((GfxSpriteLayer *)mem, 0);
        layer = (GfxSpriteLayer *)mem;
    }
    self->layer = layer;
    self->savedAlphaCount = 0x106;
    GfxSpriteLayerInitPool(layer, 0x106);
    self->layer->sorted = 1;
    self->sprites = (GfxSprite **)HudAllocLow(self->savedAlphaCount * sizeof(GfxSprite *));
    memset(self->sprites, 0, self->savedAlphaCount * sizeof(GfxSprite *));
    UiLayoutCreateSprites(self->layer, self->sprites, 4);
    for (i = 0; i < 0xbc; i++) {
        self->sprites[i]->flags &= ~HUD_SPRITE_VISIBLE;
    }
    shownSprites[0] = 0x6d;
    shownSprites[1] = 0x6a;
    shownSprites[2] = 0x6b;
    shownSprites[3] = 0x68;
    shownSprites[4] = 0x69;
    shownSprites[5] = 0xb;
    shownSprites[6] = 0xc;
    for (i = 0; i < 7; i++) {
        self->sprites[shownSprites[i]]->flags |= HUD_SPRITE_VISIBLE;
    }

    /* profile words 3..6, slot 0 replaced by the player Bakugan id; 0 counts as 1, then minus 1 */
    slotWord[0] = (s32)SaveProfileGetWord(SaveGetProfile(), 3);
    slotWord[1] = (s32)SaveProfileGetWord(SaveGetProfile(), 4);
    slotWord[2] = (s32)SaveProfileGetWord(SaveGetProfile(), 5);
    slotWord[3] = (s32)SaveProfileGetWord(SaveGetProfile(), 6);
    slotWord[0] = (s32)unit->base.base.unk08;
    for (i = 0; i < 4; i++) {
        if (slotWord[i] == 0) {
            slotWord[i] = 1;
        }
    }
    for (i = 0; i < 4; i++) {
        if (slotWord[i] != 0) {
            slotWord[i]--;
        }
    }

    /* player name plate (sprite 10) */
    sprite = self->sprites[10];
    tint[0] = 0.19921875f;
    tint[1] = 0.68359375f;
    tint[2] = 0.65234375f;
    tint[3] = 1.0f;
    HudSetColor(sprite, tint);
    GfxSpriteSetCell(self->sprites[10], 0.0f, (float)slotWord[0]);

    /* sphere icon (sprite 0x6c) for Bakugan ids 1..20 */
    if (unit != NULL) {
        if (unit->base.base.unk08 != 0 && unit->base.base.unk08 < 0x15) {
            self->sprites[10]->flags |= HUD_SPRITE_VISIBLE;
            self->sprites[0x6c]->flags |= HUD_SPRITE_VISIBLE;
            sprintf(name, "baku_sphere_%03d", unit->base.base.unk08);
            sprite = self->sprites[0x6c];
            sprite->texture = GfxFindTexture(name);
            self->sprites[0x6c]->posZ -= 20.0f;
        } else {
            self->sprites[10]->flags &= ~HUD_SPRITE_VISIBLE;
            self->sprites[0x6c]->flags &= ~HUD_SPRITE_VISIBLE;
        }
    }

    /* attribute icon (sprite 0xb); the virtual call also runs when unit is NULL (see Notes) */
    sprite = self->sprites[0xb];
    BtlHudSetAttributeIcon(self, sprite, HudUnitAttribute(unit), true);
    self->sprites[0xb]->alpha = 0.8f;

    /* status icons: clones of sprite 0xb with attribute i, rest position in iconTarget[i] */
    for (i = 0; i < 6; i++) {
        sprite = GfxSpriteLayerCloneSprite(self->layer, self->sprites[0xb]);
        self->sprites[0xe9 + i] = sprite;
        BtlHudSetAttributeIcon(self, sprite, i, true);
        sprite->posX = 64.0f;
        sprite->posY = 247.0f;
        GfxSpriteCenterPivot(sprite);
        sprite->alpha = 0.8f;
        GfxSpriteSetScaleRotation(sprite, 0.75f, 0.75f, 0.0f, false);
        sprite->flags &= ~HUD_SPRITE_VISIBLE;
        GfxSpriteInsetUv(0.5f, sprite);
        HudGetPos(self->iconTarget[i], sprite);
    }

    /* name-plate cell of the other Bakugan i (-1 none) */
    for (i = 0; i < 3; i++) {
        other = (BtlBakugan *)BtlHudGetNthOtherBakugan(self, i);
        if (other == NULL) {
            nameCell[i] = -1;
        } else {
            nameCell[i] = (s32)other->base.base.unk08;
            nameCell[i] -= 1;
            if (!(slotWord[i] < 4)) {
                nameCell[i]--;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        sprite = GfxSpriteLayerCloneSprite(self->layer, self->sprites[10]);
        self->sprites[0xbc + i] = sprite;
        self->sprites[0xbc + i]->flags &= ~HUD_SPRITE_VISIBLE;
        sprite = self->sprites[0xbc + i];
        sprite->texture = GfxFindTexture("game_baku_name");
        self->sprites[0xbc + i]->layerMask = 8;
        UiSpriteSetSize(128.0f, 16.0f, self->sprites[0xbc + i]);
        if (nameCell[i] != -1) {
            GfxSpriteSetCell(self->sprites[0xbc + i], 0.0f, (float)(nameCell[i] % 32));
        }
    }
    for (i = 0; i < 3; i++) {
        sprite = GfxSpriteLayerCloneSprite(self->layer, self->sprites[0xaf]);
        self->sprites[0xfb + i] = sprite;
        self->sprites[0xfb + i]->flags &= ~HUD_SPRITE_VISIBLE;
        self->sprites[0xfb + i]->layerMask = 8;
        UiSpriteSetSize(128.0f, 16.0f, self->sprites[0xfb + i]);
    }

    /* item icons */
    for (i = 0; i < 3; i++) {
        sprite = GfxSpriteLayerCloneSprite(self->layer, self->sprites[0xae]);
        self->sprites[0xfe + i] = sprite;
        self->sprites[0xfe + i]->flags &= ~HUD_SPRITE_VISIBLE;
        sprintf(name, "game_item_%02d", i + 1);
        sprite = self->sprites[0xfe + i];
        sprite->texture = GfxFindTexture(name);
    }

    /* enemy arrow palette blenders (CLUT row 1) */
    for (i = 0; i < 4; i++) {
        if (self->arrowBlend[i] == NULL) {
            mem = HudAllocLow(sizeof(GfxPaletteBlender));
            obj = NULL;
            if (mem != NULL) {
                GfxPaletteBlendInit(mem, 0x10, self->sprites[0xf + i * 3]->texture);
                obj = mem;
            }
            self->arrowBlend[i] = obj;
        }
        GfxPaletteBlendInstall(self->arrowBlend[i], 1, NULL);
        GfxPaletteBlendSetRow(self->arrowBlend[i], 1);
        GfxPaletteBlendSetRange(self->arrowBlend[i], 0, 0x10);
    }

    /* enemy arrow sprites 0xf..0x1a: shown position, hidden 96 px further out */
    for (i = 0; i < 12; i++) {
        sprite = self->sprites[0xf + i];
        GfxSpriteCenterPivot(sprite);
        sprite->flags &= ~HUD_SPRITE_VISIBLE;
        sprite->textureSlot = 1;
        GfxSpriteSetScaleRotation(sprite, 1.0f, 1.0f, 0.0f, false);
        GfxSpriteInsetUv(0.5f, sprite);
        sprite->posZ += 50.0f;
        HudGetPos(&self->arrowShownPos[i / 3][(i % 3) * 4], sprite);
    }
    for (i = 0; i < 12; i++) {
        HudCopyVec4(&self->arrowHiddenPos[i / 3][(i % 3) * 4],
                       &self->arrowShownPos[i / 3][(i % 3) * 4]);
        switch (i / 3) {
        case 0:
            self->arrowHiddenPos[i / 3][(i % 3) * 4 + 1] -= 96.0f;
            break;
        case 1:
            self->arrowHiddenPos[i / 3][(i % 3) * 4 + 1] += 96.0f;
            break;
        case 2:
            self->arrowHiddenPos[i / 3][(i % 3) * 4] -= 96.0f;
            break;
        case 3:
            self->arrowHiddenPos[i / 3][(i % 3) * 4] += 96.0f;
            break;
        }
    }
    for (i = 0; i < 7; i++) {
        self->sprites[3 + i]->alpha = 0.8f;
    }

    /* timer scroll bars, stencil-clipped by sprite 9 */
    frame = self->sprites[9];
    frame->alpha = 0.8f;
    GfxSpriteSetStencilWrite(frame, true, 9);
    for (i = 0; i < 2; i++) {
        pos[0] = frame->posX;
        pos[1] = -128.0f;
        pos[2] = -12.0f;
        pos[3] = 0.0f;
        sprite = GfxSpriteLayerCreateSpriteByName(self->layer, "NonTexture", pos, false);
        self->timerScroll[i] = sprite;
        GfxSpriteSetStencilTest(sprite, true, 9);
        GfxSpriteSetQuadMode(self->timerScroll[i], 2);
        self->timerScroll[i]->blendMode = 2;
        UiSpriteSetSize(128.0f, 4.0f, self->timerScroll[i]);
        tint[0] = 0.19921875f;
        tint[1] = 0.68359375f;
        tint[2] = 0.65234375f;
        tint[3] = 1.0f;
        HudSetColor(self->timerScroll[i], tint);
        self->timerScroll[i]->alpha = 0.4f;
    }
    self->timerScroll[1]->posY = self->timerScroll[0]->posY - 8.0f;

    /* lock-on marker palette blenders (CLUT row 0) on sprites 0x1b..0x1d */
    for (i = 0; i < 3; i++) {
        if (self->markerBlend[i] == NULL) {
            mem = HudAllocLow(sizeof(GfxPaletteBlender));
            obj = NULL;
            if (mem != NULL) {
                GfxPaletteBlendInit(mem, 0x10, self->sprites[0x1b + i]->texture);
                obj = mem;
            }
            self->markerBlend[i] = obj;
        }
        GfxPaletteBlendInstall(self->markerBlend[i], 5, NULL);
        GfxPaletteBlendSetRow(self->markerBlend[i], 0);
        GfxPaletteBlendSetRange(self->markerBlend[i], 0, 0x10);
    }
    for (i = 0; i < 4; i++) {
        self->sprites[0x1b + i]->flags &= ~HUD_SPRITE_VISIBLE;
    }
    for (i = 0; i < 1; i++) {
        sprite = self->sprites[0x1b + i];
        sprite->alpha = 0.5f;
        sprite->flags &= ~HUD_SPRITE_VISIBLE;
        GfxSpriteCenterPivot(sprite);
        GfxSpriteSetScaleRotation(sprite, 1.0f, 1.0f, 0.0f, false);
        sprite->textureSlot = 5;
    }

    /* combo digits 0..2 */
    for (i = 0; i < 3; i++) {
        sprite = self->sprites[i];
        GfxSpriteSetBottomCentrePivot(sprite);
        sprite->flags &= ~HUD_SPRITE_VISIBLE;
        GfxSpriteSetScaleRotation(sprite, 1.0f, 1.0f, 0.0f, false);
        HudGetPos(self->comboDigitPos[i], sprite);
    }

    /* effect sprites at sprite 0's position */
    for (i = 0; i < 2; i++) {
        sprite = GfxSpriteLayerCreateSpriteByName(self->layer, "hisan_eff", &self->sprites[0]->posX, false);
        self->sprites[0x101 + i] = sprite;
        BtlHudSetSpriteRect(0.0f, 0.0f, 40.0f, 40.0f, self, sprite);
        sprite->posZ += 4.0f;
        sprite->alpha = 0.0f;
        GfxSpriteInsetUv(8.0f, sprite);
    }
    for (i = 0; i < 2; i++) {
        sprite = GfxSpriteLayerCreateSpriteByName(self->layer, "housha_hikari", &self->sprites[0]->posX, false);
        self->sprites[0x103 + i] = sprite;
        BtlHudSetSpriteRect(0.0f, 0.0f, 64.0f, 40.0f, self, sprite);
        sprite->posZ += 4.0f;
        sprite->alpha = 0.0f;
        GfxSpriteInsetUv(4.0f, sprite);
    }
    for (i = 0x1f; i < 0x27; i++) {
        self->sprites[i]->flags &= ~HUD_SPRITE_VISIBLE;
    }
    self->sprites[0x1f]->flags &= ~HUD_SPRITE_VISIBLE;
    self->sprites[0x27]->flags &= ~HUD_SPRITE_VISIBLE;

    /* charge gauge state */
    for (i = 0; i < 3; i++) {
        HudZeroVec4(self->gaugeAnchor[i]);
        self->reserved370[i] = 0.0f;
        self->gaugeGlowAngle[i] = 0.0f;
        self->gaugeRatio[i] = 1.0f;
    }

    if (unit != NULL) {
        /* gauge anchors 0/1: sprite 0x23's position 32 px left / right */
        HudGetPos(self->gaugeAnchor[1], self->sprites[0x23]);
        HudCopyVec4(self->gaugeAnchor[0], self->gaugeAnchor[1]);
        self->gaugeAnchor[0][0] -= 32.0f;
        self->gaugeAnchor[1][0] += 32.0f;

        /* gauge sprites 0x21..0x26 at anchor (i / 2) % 3; even ones are the yellow glow */
        for (i = 0; i < 6; i++) {
            sprite = self->sprites[0x21 + i];
            HudSetPos(sprite, self->gaugeAnchor[(i / 2) % 3]);
            if (i % 2 == 0) {
                sprite->posZ -= 8.0f;
                HudSetColor(sprite, &g_colorYellow.x);
                sprite->alpha = 0.0f;
                sprite->blendMode = 2;
            }
            GfxSpriteCenterPivot(sprite);
            GfxSpriteSetScaleRotation(sprite, 1.0f, 1.0f, 0.0f, false);
            GfxSpriteInsetUv(0.5f, sprite);
        }

        /* gauge fill sprites (card_ura) */
        for (i = 0; i < 3; i++) {
            sprite = GfxSpriteLayerCreateSpriteByName(self->layer, "card_ura", self->gaugeAnchor[i], false);
            self->sprites[0xc2 + i] = sprite;
            BtlHudSetSpriteRect(0.0f, 0.0f, 48.0f, 64.0f, self, sprite);
            sprite->flags &= ~HUD_SPRITE_VISIBLE;
            GfxSpriteCenterPivot(sprite);
            GfxSpriteSetScaleRotation(sprite, 1.0f, 1.0f, 0.0f, false);
            GfxSpriteInsetUv(0.5f, sprite);
            sprite->posZ -= 16.0f;
            tint[0] = 0.5f;
            tint[1] = 0.5f;
            tint[2] = 0.5f;
            tint[3] = 1.0f;
            HudSetColor(sprite, tint);
        }
        for (i = 0x6f; i < 0x72; i++) {
            self->sprites[i]->flags &= ~HUD_SPRITE_VISIBLE;
        }

        /* the two art card slots */
        for (i = 0; i < 2; i++) {
            glow = self->sprites[0x21 + i * 2];
            art = self->sprites[0x22 + i * 2];
            gaugeFill = self->sprites[0xc2 + i];
            frame = self->sprites[0x6f + i];
            surface = self->sprites[0x72 + i];
            glow->flags |= HUD_SPRITE_VISIBLE;
            frame->flags |= HUD_SPRITE_VISIBLE;
            if (i < unit->combat.artSlotCount) {
                art->flags |= HUD_SPRITE_VISIBLE;
                gaugeFill->flags |= HUD_SPRITE_VISIBLE;
                sprintf(name, "card_L_%03d.lzs", HudArtId(unit, (u8)i) + 1);
                blob = CorePackChainFind(g_ioLzsPackages, name);
                if (blob != NULL) {
                    pixels = (u8 *)MemAllocAligned(CoreLzssGetSize((const u8 *)blob), true);
                    self->buildPixels[i] = pixels;
                    CoreLzssDecompress((const u8 *)blob, pixels);
                    sprintf(name, "card_L_%03d", HudArtId(unit, (u8)i) + 1);
                    mem = HudAllocLow(sizeof(GfxTexture));
                    obj = NULL;
                    if (mem != NULL) {
                        GfxTextureCtor((CoreObject *)mem, name, self->buildPixels[i], 1);
                        obj = mem;
                    }
                    self->buildTex[i] = (CoreObject *)obj;
                    art->texture = obj;
                }
            }
            BtlHudSetSpriteRect(0.0f, 0.0f, 128.0f, 176.0f, self, art);
            GfxSpriteSetScaleRotation(art, 0.380950004f, 0.380950004f, 0.0f, false);
            drop = (float)(0xc0 - i * 0x40);
            glow->posZ -= drop;
            art->posZ -= drop;
            gaugeFill->posZ -= drop;
            GfxSpriteCenterPivot(frame);
            HudCopyPos(frame, gaugeFill);
            frame->posX -= 2.0f;
            frame->posY += 2.0f;
            frame->posZ -= (float)(0xb6 - i * 0x40);
            GfxSpriteCenterPivot(surface);
            HudCopyPos(surface, gaugeFill);
            surface->posZ -= drop;
            surface->alpha = 0.9f;
            self->gaugeAnchor[i][0] = gaugeFill->posY;
            self->gaugeAnchor[i][1] = 203.0f;
        }
    }

    sprite = self->sprites[0x20];
    sprite->layerMask = 0x10;
    GfxSpriteCenterPivot(sprite);
    GfxSpriteSetScaleRotation(sprite, 1.0f, 1.0f, 0.0f, false);
    GfxSpriteInsetUv(0.5f, sprite);
    sprite->posZ = 0.0f;
    sprite = self->sprites[0x1f];
    sprite->layerMask = 0x10;
    GfxSpriteCenterPivot(sprite);
    GfxSpriteSetScaleRotation(sprite, 1.0f, 1.0f, 0.0f, false);
    GfxSpriteInsetUv(0.5f, sprite);
    sprite->posZ = -16.0f;
    self->sprites[0xd]->flags &= ~HUD_SPRITE_VISIBLE;
    self->sprites[0xe]->flags &= ~HUD_SPRITE_VISIBLE;
    for (i = 0x28; i < 0x5f; i++) {
        self->sprites[i]->flags &= ~HUD_SPRITE_VISIBLE;
    }
    for (i = 0x8e; i < 0x9f; i++) {
        self->sprites[i]->flags &= ~HUD_SPRITE_VISIBLE;
    }
    for (i = 0x9f; i < 0xaa; i++) {
        self->sprites[i]->flags &= ~HUD_SPRITE_VISIBLE;
    }
    for (i = 0xaa; i < 0xae; i++) {
        self->sprites[i]->flags &= ~HUD_SPRITE_VISIBLE;
    }

    /* score board (score mode) */
    if (g_scriptGlobalVars[8] == 2) {
        players = 0;
        word = (s32)SaveProfileGetWord(SaveGetProfile(), 7);
        if (0 < word && word < 3) {
            players = 4;
            for (i = 0x2a; i < 0x4c; i++) {
                self->sprites[i]->flags |= HUD_SPRITE_VISIBLE;
            }
            for (i = 0x3d; i < 0x49; i++) {
                self->sprites[i]->alpha = 0.0f;
            }
            for (i = 0; i < 9; i++) {
                BtlHudRefreshScoreCounter(self, i, -999);
            }
            for (i = 0; i < 4; i++) {
                if (0 < (s32)SaveProfileGetWord(SaveGetProfile(), i + 3)) {
                    self->sprites[0xaa + i]->flags |= HUD_SPRITE_VISIBLE;
                }
            }
            if (SaveProfileGetWord(SaveGetProfile(), 0xb) == 0xffffffffu) {
                for (i = 0x49; i < 0x4c; i++) {
                    self->sprites[i]->flags &= ~HUD_SPRITE_VISIBLE;
                    self->sprites[i]->alpha = 0.0f;
                }
                for (i = 0x3a; i < 0x3d; i++) {
                    self->sprites[i]->flags &= ~HUD_SPRITE_VISIBLE;
                    self->sprites[i]->alpha = 0.0f;
                }
            }
        }
        for (i = 0; i < players; i++) {
            sprite = BtlHudGetScoreSprite(self, 1, i);
            sprite2 = BtlHudGetScoreSprite(self, 2, i);
            HudGetPos(self->gainPopupPos[i][0], sprite);
            HudGetPos(self->gainPopupPos[i][1], sprite2);
        }
    }

    /* player-number tags of the other Bakugan (sprites 0x2b..0x2d, clones 0xbf..0xc1) */
    for (i = 0; i < 3; i++) {
        other = (BtlBakugan *)BtlHudGetNthOtherBakugan(self, i);
        sprite = GfxSpriteLayerCloneSprite(self->layer, self->sprites[0x2b + i]);
        self->sprites[0xbf + i] = sprite;
        self->sprites[0x2b + i]->flags &= ~HUD_SPRITE_VISIBLE;
        self->sprites[0xbf + i]->flags &= ~HUD_SPRITE_VISIBLE;
        self->sprites[0xbf + i]->layerMask = 8;
        if (g_scriptGlobalVars[8] == 1) {
            self->sprites[0xbf + i]->alpha = 0.0f;
        } else if (other != NULL) {
            sprintf(name, "cha_p_num_ssP%d", other->playerSlot + 1);
            sprite = self->sprites[0xbf + i];
            sprite->texture = GfxFindTexture(name);
            if (BtlIsScoreMode(2) != 0 || BtlIsScoreMode(1) != 0) {
                if (NetPlayHasManager()) {
                    sprintf(name, "cha_p_num_ssP%d", 2);
                }
                sprite = self->sprites[0x2b + i];
                sprite->texture = GfxFindTexture(name);
                self->sprites[0x2b + i]->flags |= HUD_SPRITE_VISIBLE;
            }
        }
    }
    if (multiPlayer) {
        GfxSpriteSetVCell(0.0f, self->sprites[0x2b]);
        GfxSpriteSetVCell(1.0f, self->sprites[0x2c]);
        GfxSpriteSetVCell(1.0f, self->sprites[0x2d]);
        GfxSpriteSetVCell(0.0f, self->sprites[0xbf]);
        GfxSpriteSetVCell(1.0f, self->sprites[0xc0]);
        GfxSpriteSetVCell(1.0f, self->sprites[0xc1]);
    } else {
        GfxSpriteSetVCell(1.0f, self->sprites[0x2b]);
        GfxSpriteSetVCell(2.0f, self->sprites[0x2c]);
        GfxSpriteSetVCell(2.0f, self->sprites[0x2d]);
        GfxSpriteSetVCell(1.0f, self->sprites[0xbf]);
        GfxSpriteSetVCell(2.0f, self->sprites[0xc0]);
        GfxSpriteSetVCell(2.0f, self->sprites[0xc1]);
    }

    /* screen-centre clone of sprite 0 */
    sprite = GfxSpriteLayerCloneSprite(self->layer, self->sprites[0]);
    self->sprites[0xc5] = sprite;
    sprite->flags &= ~HUD_SPRITE_VISIBLE;
    sprite->layerMask = 0x10;
    pos[0] = 240.0f;
    pos[1] = 136.0f;
    pos[2] = -200.0f;
    pos[3] = 0.0f;
    HudSetPos(sprite, pos);
    GfxSpriteCenterPivot(sprite);
    GfxSpriteSetScaleRotation(sprite, 1.0f, 1.0f, 0.0f, false);
    GfxSpriteInsetUv(1.5f, sprite);
    sprite = self->sprites[0x5f];
    sprite->flags &= ~HUD_SPRITE_VISIBLE;
    sprite->layerMask = 0x10;
    sprite->posZ = 200.0f;
    sprite->posY += 32.0f;
    sprite = self->sprites[0xba];
    sprite->flags &= ~HUD_SPRITE_VISIBLE;
    sprite->layerMask = 0x10;
    sprite->posZ = 201.0f;
    sprite->posY += 32.0f;
    for (i = 0x63; i < 0x65; i++) {
        self->sprites[i]->flags &= ~HUD_SPRITE_VISIBLE;
    }

    /* radar */
    self->sprites[0x60]->alpha = 0.9f;
    sprite = self->sprites[0x61];
    GfxSpriteSetStencilWrite(sprite, true, 5);
    sprite->alpha = 0.9f;
    self->sprites[0x62]->flags &= ~HUD_SPRITE_VISIBLE;
    self->radarScale = 0.0125f;
    texture = GfxFindTexture("NonTexture");
    pos[0] = 426.0f;
    pos[1] = 47.0f;
    pos[2] = 1.0f;
    pos[3] = 0.0f;
    sprite = GfxSpriteLayerCreateSprite(self->layer, texture, pos, false);
    self->sprites[0xc6] = sprite;
    sprite->flags &= ~HUD_SPRITE_VISIBLE;
    UiSpriteSetSize(256.0f, 256.0f, sprite);
    self->radarX = 0;
    self->radarY = 0;
    uv[0] = 0.0f;
    uv[1] = 0.0f;
    uv[2] = 16.0f;
    uv[3] = 16.0f;
    for (i = 0; i < 0x10; i++) {
        sprite = GfxSpriteLayerCloneSprite(self->layer, self->sprites[0x64]);
        self->sprites[0xc7 + i] = sprite;
        GfxSpriteSetStencilTest(sprite, false, 5);
        UiSpriteSetUvRect(sprite, uv);
        UiSpriteSetSize(16.0f, 16.0f, sprite);
        sprite->posZ = -32.0f;
        GfxSpriteSetScaleRotation(sprite, 1.0f, 1.0f, 9.99999975e-06f, false);
        GfxSpriteCenterPivot(sprite);
        sprite->posX = 426.0f;
        sprite->posY = 47.0f;
        switch (0xc7 + i) {
        case 0xc7:
            sprite->texture = GfxFindTexture("ga_rad_01");
            sprite->flags |= HUD_SPRITE_VISIBLE;
            break;
        case 0xc8:
        case 0xc9:
        case 0xca:
        case 0xcb:
            sprite->texture = GfxFindTexture("ga_rad_02");
            sprite->posZ = -33.0f;
            break;
        case 0xcc:
            sprite->texture = GfxFindTexture("ga_rad_04");
            sprite->posZ = -30.0f;
            break;
        case 0xcd:
            sprite->texture = GfxFindTexture("ga_rad_05");
            sprite->posZ = -30.0f;
            break;
        case 0xce:
            sprite->texture = GfxFindTexture("ga_rad_06");
            sprite->posZ = -30.0f;
            break;
        case 0xcf:
        case 0xd0:
        case 0xd1:
            sprite->texture = GfxFindTexture("ga_rad_07");
            sprite->tint[0] = 1.0f;
            sprite->tint[1] = 1.0f;
            sprite->tint[2] = 0.5f;
            sprite->alpha = 1.0f;
            sprite->posZ = -30.0f;
            break;
        case 0xd2:
        case 0xd3:
        case 0xd4:
        case 0xd5:
        case 0xd6:
            sprite->texture = GfxFindTexture("ga_rad_03");
            sprite->tint[0] = 1.0f;
            sprite->tint[1] = 0.200000003f;
            sprite->tint[2] = 1.0f;
            sprite->alpha = 1.0f;
            sprite->posZ = -28.0f;
            break;
        }
        if (i != 0) {
            sprite->flags &= ~HUD_SPRITE_VISIBLE;
        }
    }
    for (i = 0; i < 0x10; i++) {
        sprite = GfxSpriteLayerCloneSprite(self->layer, self->sprites[0x63]);
        self->sprites[0xd7 + i] = sprite;
        sprite->flags &= ~HUD_SPRITE_VISIBLE;
    }

    /* ability cut-in sprites */
    for (i = 0x65; i < 0x68; i++) {
        self->sprites[i]->flags &= ~HUD_SPRITE_VISIBLE;
        self->sprites[i]->layerMask = 0x20;
    }
    for (i = 0x72; i < 0x75; i++) {
        self->sprites[i]->flags &= ~HUD_SPRITE_VISIBLE;
        bits = HudRandomBits();
        self->sprites[i]->scaleX = (float)(((bits >> 16) << 3) >> 16);
    }
    sprite = self->sprites[0x65];
    GfxSpriteCenterPivot(sprite);
    GfxSpriteSetScaleRotation(sprite, 2.0f, 2.0f, 0.0f, false);
    sprite = self->sprites[0x67];
    BtlHudSetSpriteRect(0.0f, 0.0f, 256.0f, 272.0f, self, sprite);
    GfxSpriteCenterPivot(sprite);
    sprite->posZ = -200.0f;
    HudGetPos(self->cutInPortraitPos, sprite);
    sprite = GfxSpriteLayerCloneSprite(self->layer, self->sprites[0x65]);
    self->sprites[0xe8] = sprite;
    self->sprites[0xe8]->flags &= ~HUD_SPRITE_VISIBLE;
    self->sprites[0xe8]->layerMask = 0x20;
    GfxSpriteCenterPivot(self->sprites[0xe8]);
    GfxSpriteSetScaleRotation(self->sprites[0xe8], 2.0f, 2.0f, 0.0f, false);
    GfxSpriteCenterPivot(self->sprites[0xe8]);
    for (i = 0; i < 1; i++) {
        sprite = GfxSpriteLayerCloneSprite(self->layer, self->sprites[0x22]);
        self->sprites[0xe7 + i] = sprite;
        self->sprites[0xe7 + i]->flags &= ~HUD_SPRITE_VISIBLE;
        self->sprites[0xe7 + i]->layerMask = 0x20;
    }
    HudGetPos(self->cutInCardPos, self->sprites[0xe7]);
    self->cutInCardPos[0] = 358.0f;
    self->cutInCardPos[1] = 136.0f;
    self->cutInCardPos[2] = -120.0f;

    /* guard popup */
    self->sprites[0x77]->flags &= ~HUD_SPRITE_VISIBLE;
    self->sprites[0x6d]->posZ -= 10.0f;
    self->sprites[0x75]->flags &= ~HUD_SPRITE_VISIBLE;
    HudGetPos(self->guardRestPos, self->sprites[0x75]);
    self->sprites[0x76]->flags &= ~HUD_SPRITE_VISIBLE;

    /* start banner: recolour the fab_start CLUTs with the player's attribute hue */
    fab = BtlHudCreateTextObject(self, "start.fab", 0);
    fab->loop = 0;
    fab->depth = 110.0f;
    attribute = 0;
    if (unit != NULL) {
        attribute = HudUnitAttribute(unit);
    }
    startTextures[0] = "fab_start01";
    startTextures[1] = "fab_start02";
    startTextures[2] = "fab_start03";
    startTextures[3] = "fab_start04";
    hue = &g_btlHudAttributeHues[attribute];
    for (i = 0; i < 4; i++) {
        texture = GfxFindTexture(startTextures[i]);
        colorCount = 0x10;
        if (texture != NULL) {
            if (GfxTextureGetPsm(texture) == 5) {
                colorCount = 0x100;
            }
            clut = (HudClutEntry *)GfxTextureGetClutData((GfxTexture *)texture);
            for (j = 0; j < colorCount; j++) {
                alpha = clut[j].rgba[3];
                a = (float)alpha * 0.00392156979f;
                GfxHsvToRgb(*hue, (1.10000002f - a * a) * 0.909090996f, 1.0f, a, rgba);
                /* vsat0 / vscl by S701 (255) / vf2iz 23 / vi2uc: RGBA8, lane 0 in the low byte */
                packed = (u32)VfI2uc(VfF2iz(VfSat0(rgba[0]) * 255.0f, 23)) |
                         (u32)VfI2uc(VfF2iz(VfSat0(rgba[1]) * 255.0f, 23)) << 8 |
                         (u32)VfI2uc(VfF2iz(VfSat0(rgba[2]) * 255.0f, 23)) << 16 |
                         (u32)VfI2uc(VfF2iz(VfSat0(rgba[3]) * 255.0f, 23)) << 24;
                clut[j].word = packed;
                clut[j].rgba[3] = alpha;
            }
        }
    }

    /* charge gauge .fab objects */
    fab = BtlHudCreateTextObject(self, "gauge.fab", 1);
    fab->loop = 1;
    fab->depth = 90.0f;
    self->chargeFabBase = 0;
    self->chargeFabFull = GfxFabGetFrameCount(fab);
    fab = BtlHudCreateTextObject(self, "gauge_one.fab", 2);
    BtlHudSetTextObjectPos(-8.0f, 0.0f, self, 2);
    fab->loop = 1;
    fab->depth = 108.0f;
    self->chargeFullStartFrame = 0;
    self->chargeFullEndFrame = GfxFabGetFrameCount(fab);
    fab = BtlHudCreateTextObject(self, "gauge_loop_start.fab", 3);
    BtlHudSetTextObjectPos(-8.0f, 0.0f, self, 3);
    fab->loop = 1;
    fab->depth = 107.0f;
    self->chargeLoopStartFrame = 0;
    self->chargeLoopFrameCount = GfxFabGetFrameCount(fab);
    self->chargeFabRatio = (float)(self->chargeFabFull - self->chargeFabBase) * 0.00999999978f;
    BtlHudSetTextObjectPos(0.0f, 1.0f, self, 1);
    BtlHudFabSeek(self, 2, 1);

    /* talk text printers */
    mem = HudAllocLow(sizeof(UiTextPrinter));
    printer = NULL;
    if (mem != NULL) {
        UiTextPrinterCtor((UiTextPrinter *)mem, 0, NULL);
        printer = (UiTextPrinter *)mem;
    }
    self->overlayObj[0] = printer;
    printer->maxLines = 3;
    UiTextPrinterSetFont((UiTextPrinter *)self->overlayObj[0], 1);
    ((UiTextPrinter *)self->overlayObj[0])->wrapWidth = 264.0f;
    ((UiTextPrinter *)self->overlayObj[0])->widthScale = 0.600000024f;
    mem = HudAllocLow(sizeof(UiTextPrinter));
    printer = NULL;
    if (mem != NULL) {
        UiTextPrinterCtor((UiTextPrinter *)mem, 0, NULL);
        printer = (UiTextPrinter *)mem;
    }
    self->overlayObj[1] = printer;
    UiTextPrinterSetFont(printer, 1);
    ((UiTextPrinter *)self->overlayObj[1])->wrapWidth = 264.0f;
    ((UiTextPrinter *)self->overlayObj[1])->widthScale = 0.600000024f;

    /* message tables and talk window state */
    if (self->mesTable0 != NULL) {
        UiMesTableRelocate(self->mesTable0);
    }
    self->msgTable = (u32 *)SaveFindLocalizedBin("DRMesBattleCommon");
    if (self->msgTable != NULL) {
        UiMesTableRelocate((void *)self->msgTable);
    }
    strcpy(self->talkText, "");
    strcpy(self->msgText, "");
    self->talkTextAlpha = 0.0f;
    self->talkTextAlphaSet = 0.0f;
    self->talkOverlayAlpha = 0.0f;
    self->talkOverlayAlphaSet = 0.0f;
    self->talkTextWidth = 0.0f;
    self->talkTextHeight = 0.0f;
    self->talkState = 0;
    self->talkHold = 0;
    self->talkKind = -1;
    self->talkMsgId = -1;
    self->talkStyle = 0;
    self->talkFade = 0.0f;
    self->talkRamp = 0.0f;
    HudZeroVec4(self->talkTextPos);
    for (i = 0x7e; i < 0x84; i++) {
        self->sprites[i]->flags &= ~HUD_SPRITE_VISIBLE;
        self->sprites[i]->layerMask = 0x10;
    }
    for (i = 0x7e; i < 0x81; i++) {
        self->sprites[i]->flags |= 0x20;
    }
    GfxSpriteSetStencilWrite(self->sprites[0x82], true, 7);
    GfxSpriteSetStencilTest(self->sprites[0x81], true, 7);

    /* standing target markers 0x78..0x7d (one per standing target, every other sprite) */
    targets = 0;
    for (i = 0; i < 3; i++) {
        if (ActorStageObjGetStandingTarget(i) != NULL) {
            targets++;
        }
    }
    for (i = 0x78; i < 0x7e; i++) {
        self->sprites[i]->flags &= ~HUD_SPRITE_VISIBLE;
        if (i == 0x7c) {
            sprite = self->sprites[i];
            sprite->texture = GfxFindTexture("ga_rad_06");
        } else if (i == 0x7a) {
            sprite = self->sprites[i];
            sprite->texture = GfxFindTexture("ga_rad_05");
        } else if (i == 0x78) {
            sprite = self->sprites[i];
            sprite->texture = GfxFindTexture("ga_rad_04");
        }
        sprite = self->sprites[i];
        sprite->tint[0] = 1.0f;
        sprite->tint[1] = 1.0f;
        sprite->tint[2] = 0.5f;
        sprite->alpha = 1.0f;
        self->sprites[i]->layerMask = 0x10;
    }
    yOffset = (float)((3 - targets) * 0x10 + 0x90);
    for (i = 0; i < 3; i++) {
        if (ActorStageObjGetStandingTarget(i) != NULL) {
            sprite = self->sprites[0x78 + i * 2];
            sprite->posX -= 8.0f;
            sprite->posY += yOffset;
            sprite->flags |= HUD_SPRITE_VISIBLE;
        }
    }

    /* finish banner */
    HudGetPos(self->finishTarget, self->sprites[0x84]);
    self->finishTarget[1] -= 64.0f;
    HudCopyVec4(self->finishTarget2, self->finishTarget);
    self->finishTarget2[1] += 40.0f;
    for (i = 0x84; i < 0x8e; i++) {
        self->sprites[i]->flags &= ~HUD_SPRITE_VISIBLE;
    }
    for (i = 0x89; i < 0x8d; i++) {
        self->sprites[i]->posY = 158.0f;
    }
    for (i = 0; i < 4; i++) {
        sprite = GfxSpriteLayerCloneSprite(self->layer, self->sprites[0x9e]);
        self->sprites[0xf7 + i] = sprite;
        sprite->flags &= ~HUD_SPRITE_VISIBLE;
        HudSetColor(sprite, &g_colorBlack.x);
        sprite->posZ += 5.0f;
    }
    self->sprites[0x88]->posZ -= 1.0f;
    sprite = GfxSpriteLayerCloneSprite(self->layer, self->sprites[0x89]);
    self->sprites[0xef] = sprite;
    self->sprites[0xef]->flags &= ~HUD_SPRITE_VISIBLE;
    GfxSpriteSetCell(self->sprites[0xef], 0.0f, 2.0f);
    self->sprites[0xef]->layerMask = 0x20;
    self->sprites[0xef]->posZ = -64.0f;

    /* full-screen black fade sprite */
    texture = GfxFindTexture("NonTexture");
    pos[0] = 0.0f;
    pos[1] = 0.0f;
    pos[2] = 0.0f;
    pos[3] = 0.0f;
    sprite = GfxSpriteLayerCreateSprite(self->layer, texture, pos, false);
    self->sprites[0xf0] = sprite;
    sprite->flags &= ~HUD_SPRITE_VISIBLE;
    UiSpriteSetSize(512.0f, 272.0f, sprite);
    sprite->layerMask = 0x10;
    HudSetColor(sprite, &g_colorBlack.x);

    /* round banner (sprites 0x8e..0x90) */
    self->multiRoundSkip = 0;
    if (!(rounds < 3) && roundIndex + 1 == rounds) {
        self->multiRoundSkip = 1;
        word = SaveProfileGetLanguage(SaveGetProfile());
        if (word == 0xc || word == 3) {
            a = GfxSpriteGetWidth(self->sprites[0x90]);
            self->sprites[0x90]->posX = (float)(0xf0 - (s32)a / 2);
        }
    }
    if (self->multiRoundSkip == 0) {
        self->sprites[0x8e]->posX -= 56.0f;
        self->sprites[0x8f]->posX -= 16.0f;
        self->sprites[0x8f]->posY += 2.0f;
        if (roundIndex < 0) {
            cell = 0.0f;
        } else if (4 < roundIndex) {
            cell = 4.0f;
        } else {
            cell = (float)roundIndex;
        }
        GfxSpriteSetUCell(cell, self->sprites[0x8f]);
    }
    for (i = 0; i < 3; i++) {
        sprite = self->sprites[0x8e + i];
        HudGetPos(self->multiRoundPos[i], sprite);
        sprite->layerMask = 0x20;
    }

    /* round-win lamps 0xa2..0xa7 and banner parts */
    for (i = 0xa2; i < 0xa8; i++) {
        self->sprites[i]->posX += 1.0f;
    }
    shiftSprites[0] = 0xa0;
    shiftSprites[1] = 0xa5;
    shiftSprites[2] = 0xa6;
    shiftSprites[3] = 0xa7;
    shiftSprites[4] = 0xa9;
    for (i = 0; i < 5; i++) {
        self->sprites[shiftSprites[i]]->posX -= 8.0f;
    }
    if (g_scriptGlobalVars[8] == 2) {
        word = (s32)SaveProfileGetWord(SaveGetProfile(), 7);
        switch (word) {
        case 1:
            msgId = 0x369;
            break;
        case 2:
            msgId = 0x368;
            break;
        default:
            msgId = 0x367;
            break;
        }
        strcpy(self->msgText, (const char *)PspPtr(self->msgTable[msgId]));
        if (rounds < 4) {
            if (!(rounds < 3)) {
                for (i = 0x9f; i < 0xaa; i++) {
                    if (i != 0xa1 && i != 0xa4 && i != 0xa7) {
                        self->sprites[i]->flags |= HUD_SPRITE_VISIBLE;
                    }
                }
                self->sprites[0xa2]->posX += 4.0f;
                self->sprites[0xa3]->posX += 12.0f;
                self->sprites[0xa5]->posX += 4.0f;
                self->sprites[0xa6]->posX += 12.0f;
            }
        } else if (rounds == 5) {
            for (i = 0x9f; i < 0xaa; i++) {
                if (i != 0xa1) {
                    self->sprites[i]->flags |= HUD_SPRITE_VISIBLE;
                }
            }
        }
    }
    if (!multiPlayer) {
        self->sprites[0xa9]->posX -= 4.0f;
        GfxSpriteSetVCell(1.0f, self->sprites[0xa9]);
    }
    for (i = 0; i < 6; i++) {
        sprite = GfxSpriteLayerCloneSprite(self->layer, self->sprites[0xa1]);
        self->sprites[0xf1 + i] = sprite;
        HudCopyPos(self->sprites[0xf1 + i], self->sprites[0xa2 + i]);
        self->sprites[0xf1 + i]->posZ -= 10.0f;
        self->sprites[0xf1 + i]->blendMode = 2;
        self->sprites[0xf1 + i]->flags &= ~HUD_SPRITE_VISIBLE;
        self->sprites[0xf1 + i]->scaleX = 0.0f;
    }

    /* light the lamps from the rounds played so far (1 win, 2 draw, 3 loss; swapped for a net guest) */
    if (BtlCameraTaskExists()) {
        wins = 0;
        losses = 0;
        for (i = 0; i < 5; i++) {
            battle = (BtlMain *)BtlGetCameraTask();
            result = battle->roundResults[i < 0 ? 0 : (4 < i ? 4 : i)];
            if (NetPlayHasManager()) {
                if (NetPlayGetLocalSlot(NetPlayGetManager()) != 0) {
                    if (result == 1) {
                        result = 3;
                    } else if (result == 3) {
                        result = 1;
                    }
                }
            }
            if (result < 2) {
                if (0 < result && wins < 3) {
                    sprite = self->sprites[0xa2 + wins];
                    sprite->texture = GfxFindTexture("shouri_on");
                    self->sprites[0xf1 + wins]->flags |= HUD_SPRITE_VISIBLE;
                    wins++;
                }
            } else if (result < 3) {
                if (wins < 3) {
                    sprite = self->sprites[0xa2 + wins];
                    sprite->texture = GfxFindTexture("shouri_on");
                    self->sprites[0xf1 + wins]->flags |= HUD_SPRITE_VISIBLE;
                    wins++;
                }
                if (losses < 3) {
                    sprite = self->sprites[0xa5 + losses];
                    sprite->texture = GfxFindTexture("shouri_on");
                    self->sprites[0xf4 + losses]->flags |= HUD_SPRITE_VISIBLE;
                    losses++;
                }
            } else if (result < 4) {
                if (losses < 3) {
                    sprite = self->sprites[0xa5 + losses];
                    sprite->texture = GfxFindTexture("shouri_on");
                    self->sprites[0xf4 + losses]->flags |= HUD_SPRITE_VISIBLE;
                    losses++;
                }
            }
        }
    }

    /* item icons, advice and control-hint sprites */
    self->sprites[0xae]->flags &= ~HUD_SPRITE_VISIBLE;
    self->sprites[0xaf]->flags &= ~HUD_SPRITE_VISIBLE;
    self->sprites[0x6e]->flags |= HUD_SPRITE_VISIBLE;
    self->sprites[0x6e]->layerMask = 0x40;
    self->adviceThreshold = ActorStageObjCountStandingAttrLandmarks();
    self->sprites[0xb0]->posX -= 35.0f;
    self->sprites[0xb0]->posZ = -1000.0f;
    self->sprites[0xb2]->posX -= 35.0f;
    self->sprites[0xb2]->posZ -= 1100.0f;
    self->sprites[0xb1]->posX -= 10.0f;
    self->sprites[0xb1]->posZ = -1000.0f;
    GfxSpriteSetVCell(1.0f, self->sprites[0xb1]);
    self->sprites[0xb5]->posX -= 10.0f;
    self->sprites[0xb5]->posZ -= 1100.0f;
    GfxSpriteFlipU(self->sprites[0xb5]);
    self->sprites[0xb6]->posX += 50.0f;
    GfxSpriteCenterPivot(self->sprites[0xb6]);
    GfxSpriteSetScaleRotation(self->sprites[0xb6], 0.899999976f, 0.899999976f, 0.0f, false);
    GfxSpriteInsetUv(0.800000012f, self->sprites[0xb6]);
    self->sprites[0x76]->posZ = -1000.0f;
    self->phaseStep = 0;
    self->phase = self->phase + 1;
}
