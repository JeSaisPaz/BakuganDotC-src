// bdc 0x08842f38 BtlHudArenaResultScreen
#include "bdc.h"

/* Arena battle result screen, one step per frame from `BtlHudUpdateResultScreen`, state machine on
   `resultScreenState` (`resultScreenCount` frame counter, `resultScreenAngle` fade angle). Every
   frame first marks net character 0 ready when profile flag 0 is set, and while the state is > 0
   and no dialog is busy a press of pad bit 0x4000 sets `arenaSkip`; every frame ends with
   `BtlArenaResultScrollStripes`.
   - 0: creates the arena layer (0x80 bytes, 2D, sorted) and its 194-entry sprite table if missing,
     builds UI layout 0x25, saves the row rest positions, tints sprites 61..63 + 12r (r = 0..3:
     61-63, 73-75, 85-87, 97-99) with (0.3, 0.9, 0.75,
     alpha 0), counts the filled profile slots (words 3..6 > 0), updates the win/loss records when
     the camera task exists, shows and right-aligns the 16 record items, swaps the columns when
     profile word 7 is non-zero, hides rows / re-centres them for 2 or 3 filled slots, moves all 4
     rows 272 px up (off screen), loads each team's commentator photo `arena_com_pho_%03d[_%02d]` into
     sprites 24..27 (name plates 106..109) or hides them, then by profile word 7 hides the
     win (3 + 2t) / lose (4 + 2t) labels from `BtlMainGetTeamOutcome` (0) or the placing (1, 2),
     sets the rank cells of sprites 14..17, zeroes the alpha of sprites 0..111, creates the scroll
     stripes and goes to 10.
   - 10/11: fades the header in (alpha sin(angle), angle += 10 deg, clamped to [0, pi/2], jumps
     to pi/2 when `arenaSkip`), stripes at 0.4x and sprites 28/31 at 0.5x that alpha, zooms the
     layer from 0.8 to 1; at alpha 1 goes to 30.
   - 30/31: same fade at 5 deg per frame for the rows (`BtlArenaResultSetAlpha`) sliding in from
     -272 px; when done stops BGM channel 1, plays the result voice (`BtlHudPickResultVoice`),
     sets count 0 (1 when skipped, 150 when profile flag 0 is 1) and goes to 60.
   - 60: waits: without profile flag 0 counts down to -1 and stays there; with it, at 0 goes to 70
     with `arenaSkip` set. Pad bit 0x4000 (no dialog busy) goes to 70 with `arenaSkip` cleared;
     profile flag 0 with profile flags 0x4880 goes to 70 with it set.
   - 70/71: fades the layer out (layer alpha, rows sliding back to -272 px); when it reaches 0
     it falls straight through 80/81 (count 0 - 1 is never > 0) to 100 in the same frame, and 100
     activates UI window 10 (`UiSetWindowActive`) every frame.
   Other states only scroll the stripes.
   The fades take the sine on the VFPU (`vsin.s` of angle * 2/pi, bank S703), lifted to `sinf(angle)`. */

#define ARENA_HALF_PI     1.57079637f    /* 0x3fc90fdb */
#define ARENA_FADE_STEP   0.174532920f   /* 0x3e32b8c2, 10 degrees */
#define ARENA_ROW_STEP    0.0872664601f  /* 0x3db2b8c2, 5 degrees */
#define ARENA_DROP_Y      -272.0f        /* 0xc3880000 */

/* sin(angle): the VFPU's vsin.s of angle * 2/pi (quarter turns). */
static inline float ArenaSin(float angle)
{
    return __builtin_sinf(angle);
}

/* Clamps the angle to [0, pi/2] (a NaN becomes pi/2). */
static inline float ArenaClampAngle(float angle)
{
    if (angle < 0.0f) {
        return 0.0f;
    }
    if (!(angle <= ARENA_HALF_PI)) {
        return ARENA_HALF_PI;
    }
    return angle;
}

void BtlHudArenaResultScreen(BtlHud *self)
{
    char name[64];
    float cells[8];
    s32 rowSprites23[8];
    s32 firstItem[16];
    s32 headerSprites[4];
    NetChara *chara;
    GfxSpriteLayer *layer;
    GfxSprite **sprites;
    GfxSprite *sprite;
    BtlBakugan *unit;
    bool fromLow;
    float t;
    float dy;
    s32 filled;
    s32 firsts;
    s32 mode;
    s32 multi;
    s32 voice;
    s32 dup;
    bool tie;
    int outcome;
    int i;
    int j;

    if (SaveGetProfileFlag0()) {
        chara = NetCharaGetByIndex(0);
        if (chara != NULL) {
            NetCharaSetReady(chara);
        }
    }
    if (self->resultScreenState > 0 && !BtlIsDialogBusy() && (self->pad->pressed & 0x4000) != 0) {
        self->arenaSkip = 1;
    }

    switch (self->resultScreenState) {
    case 0:
        if (self->arenaLayer == NULL) {
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            layer = MemAlloc(0x80, NULL, 0);
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            if (layer != NULL) {
                GfxSpriteLayerCtor(layer, 0);
            }
            self->arenaLayer = layer;
        }
        self->arenaLayer->sorted = 1;
        if (self->arenaSprites == NULL) {
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            sprites = MemAlloc(194 * sizeof(GfxSprite *), NULL, 0); /* 0x308 bytes */
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            self->arenaSprites = sprites;
        }
        UiLayoutCreateSprites(self->arenaLayer, self->arenaSprites, 0x25);
        BtlArenaResultSaveRowPositions(self, true);

        /* Tint (0.3, 0.9, 0.75) and alpha 0 on sprites 61 + 12i + j (j = 0..2), one 16-byte copy each. */
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 3; j++) {
                sprite = self->arenaSprites[61 + i * 12 + j];
                sprite->tint[0] = 0.300000012f;
                sprite->tint[1] = 0.899999976f;
                sprite->tint[2] = 0.75f;
                sprite->alpha = 0.0f;
            }
        }

        filled = 0;
        for (i = 0; i < 4; i++) {
            if ((s32)SaveProfileGetWord(SaveGetProfile(), 3 + i) > 0) {
                filled++;
            }
        }
        if (BtlCameraTaskExists()) {
            BtlMainUpdateWinLossRecords(BtlGetCameraTask());
        }
        for (i = 0; i < 16; i++) {
            BtlResultShowScoreItem(self, i, -999);
            BtlArenaResultAlignNumber(self, self->arenaSprites, i);
        }
        if (SaveProfileGetWord(SaveGetProfile(), 7) != 0) {
            BtlArenaResultSwapColumns(self);
        }

        if (filled == 2) {
            BtlArenaResultShowRow(self, 2, false);
            BtlArenaResultShowRow(self, 3, false);
            BtlArenaResultOffsetRow(66.0f, 0.0f, self, 0);
            BtlArenaResultOffsetRow(170.0f, 0.0f, self, 1);
            BtlArenaResultSaveRowPositions(self, false);
            for (i = 18; i < 22; i++) {
                self->arenaSprites[i]->flags &= ~1u;
            }
            rowSprites23[0] = 40;
            rowSprites23[1] = 82;
            rowSprites23[2] = 83;
            rowSprites23[3] = 84;
            rowSprites23[4] = 44;
            rowSprites23[5] = 94;
            rowSprites23[6] = 95;
            rowSprites23[7] = 96;
            for (i = 0; i < 8; i++) {
                self->arenaSprites[rowSprites23[i]]->flags &= ~1u;
            }
        } else {
            if (filled == 3) {
                BtlArenaResultShowRow(self, 3, false);
                BtlArenaResultOffsetRow(56.0f, 0.0f, self, 0);
                BtlArenaResultOffsetRow(56.0f, 0.0f, self, 1);
                BtlArenaResultOffsetRow(56.0f, 0.0f, self, 2);
                BtlArenaResultSaveRowPositions(self, false);
            }
            for (i = 22; i < 24; i++) {
                self->arenaSprites[i]->flags &= ~1u;
            }
        }
        for (i = 0; i < 4; i++) {
            BtlArenaResultOffsetRow(0.0f, ARENA_DROP_Y, self, i);
        }
        GfxSpriteSetCell(self->arenaSprites[110], 0.0f, 2.0f);

        /* Commentator photo (sprites 24..27) and name plate (106..109) of each team. */
        for (i = 0; i < 4; i++) {
            unit = BtlFindTeamBakugan(i);
            if (unit == NULL) {
                continue;
            }
            if (unit->base.base.unk08 != 0 && unit->base.base.unk08 < 21) {
                if (BtlBakuganGetSameKindIndex(BtlFindTeamBakugan(i)) == 0) {
                    sprintf(name, "arena_com_pho_%03d", unit->base.base.unk08);
                } else {
                    dup = BtlBakuganGetSameKindIndex(unit);
                    sprintf(name, "arena_com_pho_%03d_%02d", unit->base.base.unk08, dup);
                }
                sprite = self->arenaSprites[24 + i];
                sprite->texture = GfxFindTexture(name);
                BtlHudSetSpriteRect(0.0f, 0.0f, 112.0f, 136.0f, self, self->arenaSprites[24 + i]);
                GfxSpriteSetVCell((float)(unit->base.base.unk08 - 1), self->arenaSprites[106 + i]);
            } else {
                self->arenaSprites[24 + i]->flags &= ~1u;
                self->arenaSprites[106 + i]->flags &= ~1u;
            }
        }

        /* Win (3 + 2t) / lose (4 + 2t) labels. */
        mode = (s32)SaveProfileGetWord(SaveGetProfile(), 7);
        if (mode == 0) {
            if (BtlCameraTaskExists()) {
                for (i = 0; i < 4; i++) {
                    outcome = BtlMainGetTeamOutcome(BtlGetCameraTask(), i);
                    if (outcome == 1) {
                        self->arenaSprites[4 + i * 2]->flags &= ~1u;
                    } else if (outcome == 2) {
                        self->arenaSprites[3 + i * 2]->flags &= ~1u;
                    } else if (outcome == 4) {
                        self->arenaSprites[3 + i * 2]->flags &= ~1u;
                        GfxSpriteSetVCell(1.0f, self->arenaSprites[4 + i * 2]);
                    }
                }
            }
            memcpy(firstItem, g_btlArenaResultFirstItemSprites, sizeof(firstItem));
            for (i = 0; i < 16; i++) {
                self->arenaSprites[firstItem[i]]->flags &= ~1u;
            }
            for (i = 0; i < 4; i++) {
                GfxSpriteSetVCell(2.0f, self->arenaSprites[33 + i * 4]);
            }
        } else if (mode > 0 && mode < 3) {
            if (BtlCameraTaskExists()) {
                filled = 0;
                tie = false;
                for (i = 0; i < 4; i++) {
                    if ((s32)SaveProfileGetWord(SaveGetProfile(), 3 + i) > 0) {
                        filled++;
                    }
                }
                firsts = 0;
                for (i = 0; i < 4; i++) {
                    if (BtlMainGetPlayerPlacing(BtlGetCameraTask(), i) == 0) {
                        firsts++;
                    }
                }
                if (filled == firsts) {
                    tie = true;
                }
                if (SaveProfileGetWord(SaveGetProfile(), 7) == 1) {
                    for (i = 0; i < 4; i++) {
                        GfxSpriteSetVCell(3.0f, self->arenaSprites[32 + i * 4]);
                    }
                }
                for (i = 0; i < 4; i++) {
                    if (BtlMainGetPlayerPlacing(BtlGetCameraTask(), i) != 0) {
                        self->arenaSprites[3 + i * 2]->flags &= ~1u;
                    } else if (tie) {
                        self->arenaSprites[3 + i * 2]->flags &= ~1u;
                        GfxSpriteSetVCell(1.0f, self->arenaSprites[4 + i * 2]);
                    } else {
                        self->arenaSprites[4 + i * 2]->flags &= ~1u;
                    }
                }
            }
        }

        /* Rank cells of sprites 14..17: rows 0..3 use cells[multi + 2 * row]. */
        cells[0] = 0.0f;
        cells[1] = 0.0f;
        cells[2] = 2.0f;
        cells[3] = 1.0f;
        cells[4] = 3.0f;
        cells[5] = 5.0f;
        cells[6] = 4.0f;
        cells[7] = 6.0f;
        multi = 0;
        if (BtlCountPlayerBakugan() >= 2) {
            multi = 1;
        }
        for (i = 0; i < 4; i++) {
            GfxSpriteSetCell(self->arenaSprites[14 + i], 0.0f, cells[multi + i * 2]);
        }
        for (i = 0; i < 112; i++) {
            self->arenaSprites[i]->alpha = 0.0f;
        }
        BtlArenaResultInitScrollStripes(self);
        self->resultScreenState = 10;
        /* fallthrough */
    case 10:
        self->resultScreenAngle = self->arenaSkip ? ARENA_HALF_PI : 0.0f;
        self->resultScreenState++;
        /* fallthrough */
    case 11:
        self->resultScreenAngle = self->resultScreenAngle + ARENA_FADE_STEP;
        if (self->arenaSkip) {
            self->resultScreenAngle = ARENA_HALF_PI;
        }
        self->resultScreenAngle = ArenaClampAngle(self->resultScreenAngle);
        t = ArenaSin(self->resultScreenAngle);
        headerSprites[0] = 1;
        headerSprites[1] = 2;
        headerSprites[2] = 110;
        headerSprites[3] = 111;
        for (i = 0; i < 4; i++) {
            sprite = self->arenaSprites[headerSprites[i]];
            sprite->alpha = t;
        }
        for (i = 0; i < 41; i++) {
            self->arenaSprites[112 + i]->alpha = t * 0.400000006f;
            self->arenaSprites[153 + i]->alpha = t * 0.400000006f;
        }
        self->arenaSprites[31]->alpha = t * 0.5f;
        self->arenaSprites[28]->alpha = t * 0.5f;
        /* Result label by profile word 7; for any other value `sprite` still holds
           arenaSprites[111] from the header loop, which gets the same alpha again. */
        mode = (s32)SaveProfileGetWord(SaveGetProfile(), 7);
        if (mode == 0) {
            sprite = self->arenaSprites[12];
        } else if (mode == 1) {
            sprite = self->arenaSprites[13];
        } else if (mode == 2) {
            sprite = self->arenaSprites[11];
        }
        if (sprite != NULL) {
            sprite->alpha = t;
        }
        GfxSpriteLayerSetZoom(t * 0.200000003f + 0.800000012f, 0.0f, self->arenaLayer, NULL);
        if (t < 1.0f) {
            break;
        }
        GfxSpriteLayerSetZoom(1.0f, 0.0f, self->arenaLayer, NULL);
        self->resultScreenState = 30;
        /* fallthrough */
    case 30:
        self->resultScreenAngle = 0.0f;
        self->resultScreenState++;
        /* fallthrough */
    case 31:
        self->resultScreenAngle = self->resultScreenAngle + ARENA_ROW_STEP;
        if (self->arenaSkip) {
            self->resultScreenAngle = ARENA_HALF_PI;
        }
        self->resultScreenAngle = ArenaClampAngle(self->resultScreenAngle);
        t = ArenaSin(self->resultScreenAngle);
        BtlArenaResultSetAlpha(t, self);
        dy = (1.0f - t) * ARENA_DROP_Y;
        for (i = 0; i < 4; i++) {
            BtlArenaResultOffsetRow(0.0f, dy, self, i);
        }
        if (t < 1.0f) {
            break;
        }
        SndBgmQueueStop(0.0f, 1);
        voice = BtlHudPickResultVoice(self);
        if (voice != -1) {
            SndBgmQueuePlay(1, voice, 0, 0);
        }
        BtlArenaResultSetAlpha(1.0f, self);
        for (i = 0; i < 4; i++) {
            BtlArenaResultOffsetRow(0.0f, 0.0f, self, i);
        }
        self->resultScreenCount = 0;
        self->resultScreenState = 60;
        if (self->arenaSkip) {
            self->arenaSkip = 0;
            self->resultScreenCount = 1;
        }
        if (SaveGetProfileFlag0() == 1) {
            self->resultScreenCount = 150;
        }
        break;
    case 60:
        if (SaveGetProfileFlag0() == 0) {
            self->resultScreenCount = self->resultScreenCount - 1;
            if (self->resultScreenCount >= 0) {
                break;
            }
            self->resultScreenCount = -1;
        } else {
            self->resultScreenCount = self->resultScreenCount - 1;
            if (self->resultScreenCount <= 0) {
                self->resultScreenState = 70;
                self->arenaSkip = 1;
            }
        }
        if (!BtlIsDialogBusy() && (self->pad->pressed & 0x4000) != 0) {
            self->resultScreenState = 70;
            self->arenaSkip = 0;
        }
        if (SaveGetProfileFlag0() && SaveHasProfile() &&
            SaveProfileHasFlags(SaveGetProfile(), 0x4880)) {
            self->resultScreenState = 70;
            self->arenaSkip = 1;
        }
        break;
    case 70:
        self->resultScreenCount = 0;
        self->resultScreenAngle = ARENA_HALF_PI;
        self->resultScreenState++;
        /* fallthrough */
    case 71:
        self->resultScreenAngle = self->resultScreenAngle + -ARENA_FADE_STEP;
        self->resultScreenAngle = ArenaClampAngle(self->resultScreenAngle);
        t = ArenaSin(self->resultScreenAngle);
        self->arenaLayer->alpha = t;
        dy = (1.0f - t) * ARENA_DROP_Y;
        for (i = 0; i < 4; i++) {
            BtlArenaResultOffsetRow(0.0f, dy, self, i);
        }
        if (!(t <= 0.0f)) {
            break;
        }
        self->arenaLayer->alpha = 0.0f;
        self->resultScreenState = 80;
        /* fallthrough */
    case 80:
        self->resultScreenCount = 0;
        self->resultScreenState++;
        /* fallthrough */
    case 81:
        self->resultScreenCount = self->resultScreenCount - 1;
        if (self->resultScreenCount > 0) {
            break;
        }
        self->resultScreenState = 100;
        /* fallthrough */
    case 100:
        UiSetWindowActive(10, 1);
        break;
    default:
        break;
    }
    BtlArenaResultScrollStripes(self);
}
