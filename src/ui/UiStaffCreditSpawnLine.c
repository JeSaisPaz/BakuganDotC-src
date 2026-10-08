// bdc 0x089454e0 UiStaffCreditSpawnLine
#include "bdc.h"

/* Emits the next line of the credits roll of `UiStaffCredit` once `rollFrame`
   reaches `lineCursor`: line `lineIndex` (< `lineCount`, < 0x1f6) of `g_staffCreditLayout`
   `lines` selects `{style, size, kind}`. Kind 9 prints `lineTexts[lineIndex]` into the first free
   overlay printer (`glyphs` NULL) at scale `g_staffCreditStyleFontSize`[style] / 24, with a
   shadow pass whose colours depend on the style, places it at Y 288 and advances `lineCursor` by
   `sizeAdvance[size]` + the font size. Other kinds show the screen sprite
   `g_staffCreditKindSprite`[kind] (kind 8 → sprite 0x1e); for sprites 0x1e / 0x18 it is first
   sized to the preceding text line (or to sprite 23 + 40) and centred on X 240; the sprite is made
   visible at Y 289 with a white tint and `lineCursor` advances by `sizeAdvance[size]` + its height.
   Recurses while the next line's sprite is 0x18 (same row). */

typedef void (*UiStaffCreditPrintFn)(float x, float y, float z, void *self, char *text, s32 a,
                                     s32 b, s32 c);

static void UiStaffCreditPrint(GfxSpriteLayer *layer, float x, float y, char *text)
{
  const VtblEntry *entry = &layer->vtbl[2];
  ((UiStaffCreditPrintFn)entry->fn)(x, y, 0.0f, (u8 *)layer + entry->delta, text, 1, 0, 0);
}

static void UiStaffCreditSetOutline(UiTextPrinter *printer, const ScePspFVector4 *color)
{
  printer->outlineColor[0] = color->x;
  printer->outlineColor[1] = color->y;
  printer->outlineColor[2] = color->z;
  printer->outlineColor[3] = color->w;
}

void UiStaffCreditSpawnLine(UiScreen *screen)
{
  UiStaffCredit *credit = (UiStaffCredit *)screen;
  GfxSprite **sprites;
  GfxSprite *sprite;
  int slot;
  int idx;
  float scale;
  float y;
  float h;
  float w;
  float size;
  float adv;

  if (!(credit->lineIndex < credit->lineCount)) {
    return;
  }
  if (credit->rollFrame < credit->lineCursor) {
    return;
  }
  if (!(credit->lineIndex < 0x1f6)) {
    return;
  }

  if (g_staffCreditLayout.lines[credit->lineIndex].kind == 9) {
    credit->lineCursor =
        (int)(g_staffCreditLayout.sizeAdvance[g_staffCreditLayout.lines[credit->lineIndex].size] +
              g_staffCreditStyleFontSize[g_staffCreditLayout.lines[credit->lineIndex].style] +
              (float)credit->lineCursor);
    /* No free printer leaves slot 20 (reads past the array, as the original does). */
    for (slot = 0; slot < 20; slot++) {
      if (((UiTextPrinter *)credit->overlays[slot])->glyphs == NULL) {
        break;
      }
    }
    scale = g_staffCreditStyleFontSize[g_staffCreditLayout.lines[credit->lineIndex].style] *
            0.041666668f;
    ((UiTextPrinter *)credit->overlays[slot])->scale = scale;
    ((UiTextPrinter *)credit->overlays[slot])->widthScale = scale;

    y = 0.0f;
    if (g_staffCreditLayout.lines[credit->lineIndex].style == 0) {
      UiStaffCreditSetOutline((UiTextPrinter *)credit->overlays[slot], &g_colorBlack);
      UiStaffCreditPrint(credit->overlays[slot], 242.0f, 2.0f,
                         (char *)PspPtr(credit->lineTexts[credit->lineIndex]));
      UiStaffCreditSetOutline((UiTextPrinter *)credit->overlays[slot], &g_colorYellow);
    } else if (g_staffCreditLayout.lines[credit->lineIndex].style == 1) {
      UiStaffCreditSetOutline((UiTextPrinter *)credit->overlays[slot], &g_colorBlack);
      UiStaffCreditPrint(credit->overlays[slot], 241.0f, 1.0f,
                         (char *)PspPtr(credit->lineTexts[credit->lineIndex]));
      UiStaffCreditSetOutline((UiTextPrinter *)credit->overlays[slot], &g_colorWhite);
    } else if (g_staffCreditLayout.lines[credit->lineIndex].style == 2) {
      y = 0.0f + 1.0f;
      UiStaffCreditSetOutline((UiTextPrinter *)credit->overlays[slot], &g_colorWhite);
      UiStaffCreditPrint(credit->overlays[slot], 241.0f, y + 1.0f,
                         (char *)PspPtr(credit->lineTexts[credit->lineIndex]));
      UiStaffCreditSetOutline((UiTextPrinter *)credit->overlays[slot], &g_colorOrange);
    }
    UiStaffCreditPrint(credit->overlays[slot], 240.0f, y, (char *)PspPtr(credit->lineTexts[credit->lineIndex]));
    credit->overlays[slot]->view.w.y = 288.0f;
  } else {
    idx = g_staffCreditKindSprite[g_staffCreditLayout.lines[credit->lineIndex].kind];
    if (g_staffCreditLayout.lines[credit->lineIndex].kind == 8) {
      idx = 0x1e;
    }
    if (idx == 0x1e || idx == 0x18) {
      h = 16.0f;
      if (g_staffCreditLayout.lines[credit->lineIndex - 1].kind == 9) {
        if (g_staffCreditLayout.lines[credit->lineIndex - 1].style == 2) {
          h = h + 4.0f;
        }
        size = g_staffCreditStyleFontSize[g_staffCreditLayout.lines[credit->lineIndex - 1].style];
        w = (float)UiTextMeasure(0.0f, credit->overlays[0],
                                 (char *)PspPtr(credit->lineTexts[credit->lineIndex - 1]), NULL, NULL, NULL) *
                size +
            5.0f;
        UiSpriteSetSize(w, h, ((GfxSprite **)screen->data)[idx]);
        ((GfxSprite **)screen->data)[idx]->posX = 240.0f - w * 0.5f;
      } else {
        sprites = (GfxSprite **)screen->data;
        sprite = sprites[idx];
        w = GfxSpriteGetWidth(sprites[23]);
        UiSpriteSetSize(w + 40.0f, h, sprite);
        w = GfxSpriteGetWidth(((GfxSprite **)screen->data)[idx]);
        ((GfxSprite **)screen->data)[idx]->posX = 240.0f - w * 0.5f;
      }
    }
    ((GfxSprite **)screen->data)[idx]->flags |= 1;
    ((GfxSprite **)screen->data)[idx]->posY = 289.0f;
    sprite = ((GfxSprite **)screen->data)[idx];
    sprite->tint[0] = 1.0f;
    sprite->tint[1] = 1.0f;
    sprite->tint[2] = 1.0f;
    sprite->alpha = 1.0f;
    adv = g_staffCreditLayout.sizeAdvance[g_staffCreditLayout.lines[credit->lineIndex].size];
    h = GfxSpriteGetHeight(((GfxSprite **)screen->data)[idx]);
    credit->lineCursor = (int)(adv + h + (float)credit->lineCursor);
  }

  credit->lineIndex++;
  if (g_staffCreditKindSprite[g_staffCreditLayout.lines[credit->lineIndex].kind] == 0x18) {
    UiStaffCreditSpawnLine(screen);
  }
}
