// bdc 0x08818b70 UiTextPrinterPrint
#include "bdc.h"

/* Print method of the text printer (vtable slot 2, called by `UiTextBoxPrint`). Walks `text` from
   byte `startPos` glyph by glyph (`UiFontNextGlyph`) with the pen at (`x`, `y`), creating one
   sprite per glyph (`GfxSpriteLayerCreateTemplateSprite`, cell from `fontTextures`, tinted with
   `outlineColor`). Before each break opportunity (`UiTextCanBreakBefore`) the next word is
   measured and wrapped (as code -3) when it would end past `x + wrapWidth - margin`; once
   `maxLines` lines are full the text is cut (-4), `resumePos` is set and the function returns
   whether nothing printable was left. Inline codes: -17 space, -5 half space, -11..-7 preset
   colours, -6 restore `color`, -16 sets `ctrlF0Flag`; an active insert text is printed first.
   `centerH` centres every line on `x`; `centerV` centres the measured block (`UiTextMeasure`)
   vertically on `y`. Returns 1 unless the text was cut with glyphs remaining. */

/* floor(v) as the asm computes it: vf2id.s (round towards -inf) then vi2f.s back to float */
static inline float UiTextPrinterFloor(float v)
{
  return (float)(int)floorf(v);
}

/* vec4 copy src -> dst (lv.q/sv.q in the asm) */
static inline void UiTextPrinterCopyQuad(float *dst, const float *src)
{
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

/* Shifts the sprites of the current line by `off`. */
static inline void UiTextPrinterShiftLine(GfxSprite *sprite, float off)
{
  for (; sprite != NULL; sprite = sprite->next) {
    sprite->posX = sprite->posX + off;
  }
}

bool UiTextPrinterPrint(float x, float y, float margin, UiTextPrinter *self, char *text, u8 centerH,
                        int startPos, u8 centerV)
{
  float pen[4];
  float rect[4];
  float scaledRect[4];
  float width;
  float height;
  float wordPen[4];
  float restPen[4];
  float u;
  float v;
  int pos;
  int insertPos;
  int peek;
  int wordPos;
  int wordGlyphs;
  int restPos;
  int restGlyphs;
  int glyph;
  int g;
  int lineCount;
  int perRow;
  int perCol;
  int perPage;
  int slot;
  bool truncated;
  bool result;
  void *texture;
  GfxSprite *sprite;
  GfxSprite *lineFirst;

  width = 0.0f;
  height = 0.0f;
  lineFirst = NULL;
  lineCount = 0;
  result = 1;
  self->glyphs = NULL;
  self->glyphCount = 0;
  truncated = 0;
  if (centerH != 0) {
    UiTextMeasure(0.0f, self, text, &height, &width, NULL);
    if (self->widthTable != NULL) {
      width = g_uiTextMeasureBreakWidth;
    }
    else {
      width = width * self->advanceX;
    }
  }
  (void)width; /* measured width is stored but never used */
  if (centerV != 0) {
    height = height * self->scale;
    y = y - UiTextPrinterFloor(height * 0.5f + 0.5f);
  }
  pen[1] = y;
  pen[0] = x;
  pen[2] = 0.0f;
  pos = startPos;
  pen[3] = 0.0f;
  insertPos = 0;

  for (;;) {
    if (self->inInsert != 0) {
      glyph = -1;
      if (self->insertText != NULL) {
        glyph = UiFontNextGlyph((char *)self->insertText, &insertPos);
      }
      if (glyph == -1) {
        self->inInsert = 0;
        self->ctrlF0Flag = 0;
        continue;
      }
    }
    else {
      glyph = UiFontNextGlyph(text, &pos);
      if (glyph == -12 || glyph == -11) {
        text = text + 1; /* as compiled: the base pointer moves, `pos` is kept */
        continue;
      }
      peek = pos;
      if (UiTextCanBreakBefore(glyph, UiFontNextGlyph(text, &peek)) != 0) {
        /* measure the following word */
        wordPos = pos;
        wordGlyphs = 0;
        UiTextPrinterCopyQuad(wordPen, pen);
        wordPen[0] = wordPen[0] + UiFontGlyphAdvance(self, 0);
        for (;;) {
          g = UiFontNextGlyph(text, &wordPos);
          if (g <= 0) {
            if (!SaveLanguageIsFrench() || g != -5) {
              break;
            }
            peek = wordPos;
            if (UiTextCanBreakBefore(g, UiFontNextGlyph(text, &peek)) != 0) {
              break;
            }
          }
          wordGlyphs = wordGlyphs + 1;
          if (g == -5) {
            wordPen[0] = wordPen[0] + UiFontGlyphAdvance(self, 0);
          }
          else {
            wordPen[0] = wordPen[0] + UiTextPrinterFloor(UiFontGlyphAdvance(self, g));
          }
        }
        if (wordGlyphs > 0 && (int)(x + self->wrapWidth - margin) < (int)wordPen[0]) {
          glyph = -3;
        }
      }
      if (self->maxLines > 0 && glyph == -3 && !(lineCount + 1 < self->maxLines)) {
        truncated = 1;
        glyph = -4;
      }
      if (glyph == -1) {
        break;
      }
    }

    if (glyph == -2) {
      continue;
    }
    if (glyph == -3 || glyph == -4) {
      /* new line */
      if (centerH != 0) {
        UiTextPrinterShiftLine(lineFirst, (x - pen[0]) * 0.5f);
        lineFirst = NULL;
      }
      pen[1] = pen[1] + self->lineHeight * self->scale;
      pen[0] = x;
      if (self->inInsert != 0) {
        self->inInsert = 0;
        self->ctrlF0Flag = 0;
      }
      if (glyph == -3) {
        lineCount = lineCount + 1;
      }
      if (glyph == -4) {
        lineCount = 0;
      }
      if (truncated) {
        /* count what is left (`restPen` is advanced but never read) */
        restPos = pos;
        restGlyphs = 0;
        UiTextPrinterCopyQuad(restPen, pen);
        restPen[0] = restPen[0] + UiFontGlyphAdvance(self, 0);
        while (UiFontNextGlyph(text, &restPos) >= 0) {
          restGlyphs = restGlyphs + 1;
        }
        if (restGlyphs > 0) {
          result = 0;
        }
        self->resumePos = pos;
        return result;
      }
      continue;
    }
    if (glyph == -17) {
      pen[0] = pen[0] + self->advanceX;
      continue;
    }
    if (glyph == -5) {
      if (self->widthTable != NULL) {
        pen[0] = pen[0] + UiFontGlyphAdvance(self, 0);
      }
      else {
        pen[0] = pen[0] + self->advanceX * 0.5f;
      }
      continue;
    }
    if (glyph < -5 && glyph >= -11) {
      switch (glyph) {
      case -11:
        self->outlineColor[0] = 0.0f;
        self->outlineColor[1] = 0.0f;
        self->outlineColor[2] = 1.0f;
        break;
      case -10:
        self->outlineColor[0] = 0.0f;
        self->outlineColor[1] = 1.0f;
        self->outlineColor[2] = 0.0f;
        break;
      case -9:
        self->outlineColor[0] = 1.0f;
        self->outlineColor[1] = 0.0f;
        self->outlineColor[2] = 0.0f;
        break;
      case -8:
        self->outlineColor[0] = 0.0f;
        self->outlineColor[1] = 0.0f;
        self->outlineColor[2] = 0.0f;
        break;
      case -7:
        self->outlineColor[0] = 1.0f;
        self->outlineColor[1] = 1.0f;
        self->outlineColor[2] = 1.0f;
        break;
      default: /* -6 */
        UiTextPrinterCopyQuad(self->outlineColor, self->color);
        break;
      }
      continue;
    }
    if (glyph == -16) {
      self->ctrlF0Flag = 1;
      continue;
    }
    if (glyph < 0) {
      continue;
    }

    /* printable glyph: one sprite from the font sheet */
    perRow = (int)(0x200u / (u32)self->cellW);
    perCol = (int)(0x200u / (u32)self->cellH);
    slot = 0;
    perPage = perRow * perCol;
    texture = self->fontTextures[glyph / perPage];
    sprite = GfxSpriteLayerCreateTemplateSprite(&self->layer, texture, pen);
    if (self->glyphs == NULL) {
      self->glyphs = sprite;
    }
    glyph = glyph % perPage;
    self->glyphCount = self->glyphCount + 1;
    u = (float)(u32)((glyph % perRow) * (u32)self->cellW);
    v = (float)(u32)((glyph / perCol) * (u32)self->cellH);
    sprite->texture = texture;
    UiSpriteSetSize(self->cellW, self->cellH, sprite);
    if (self->scale == 1.0f) {
      rect[0] = u;
      rect[1] = v;
      rect[2] = self->cellW;
      rect[3] = self->cellH;
      GfxSpriteSetUvRectXYWH(sprite, rect);
    }
    else {
      sprite->flags = sprite->flags | 0x20;
      GfxSpriteSetScaleRotation(sprite, self->scale, self->scale, 0.0f, false);
      scaledRect[0] = u + 1.0f;
      scaledRect[1] = v + 1.0f;
      scaledRect[2] = self->cellW - 1.0f;
      scaledRect[3] = self->cellH - 1.0f;
      GfxSpriteSetUvRectXYWH(sprite, scaledRect);
    }
    if (glyph >= 0x206) {
      slot = 1;
    }
    sprite->textureSlot = slot;
    sprite->tint[0] = self->outlineColor[0];
    sprite->tint[1] = self->outlineColor[1];
    sprite->tint[2] = self->outlineColor[2];
    sprite->alpha = self->outlineColor[3];
    if (centerH != 0 && lineFirst == NULL) {
      lineFirst = sprite;
    }
    if (self->scale == 1.0f) {
      pen[0] = UiTextPrinterFloor(pen[0]);
      pen[0] = pen[0] + UiTextPrinterFloor(UiFontGlyphAdvance(self, glyph));
    }
    else {
      pen[0] = pen[0] + UiFontGlyphAdvance(self, glyph);
    }
  }

  if (centerH != 0) {
    UiTextPrinterShiftLine(lineFirst, (x - pen[0]) * 0.5f);
  }
  return result;
}
