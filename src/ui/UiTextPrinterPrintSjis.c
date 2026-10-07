// bdc 0x08817da0 UiTextPrinterPrintSjis
#include "bdc.h"

/* Prints `text` with the printer at (`x`, `y`), one sprite per glyph, stopping once the pen passes
   x = 480. With `sjisFlag` clear the text is Shift-JIS: decoded by `UiFontNextGlyphSjis` into
   12 px high glyphs from 42-per-row, 0x6e4-per-page font sheets (pages >= 3 print
   `g_uiSjisFallbackGlyph`), newline adds 13 px. With `sjisFlag` set it is single-byte text from
   the `cellW` x `cellH` sheet `fontTextures[0]`, tinted with `outlineColor`, newline adds
   `lineHeight`, advance from `widthTable` or `advanceX`. */

void UiTextPrinterPrintSjis(float x, float y, UiTextPrinter *self, char *text)
{
  int perRow;
  int pos;
  int glyph;
  int page;
  int fallbackPos;
  void *texture;
  GfxSprite *sprite;
  float glyphW;
  float advance;
  float pen[4] __attribute__((aligned(16)));
  float rect[4];
  float sjisRect[4];

  perRow = 0x200 / (int)self->cellW;
  pen[0] = (float)(int)x;
  pen[1] = (float)(int)y;
  pen[2] = 0.0f;
  pen[3] = 0.0f;
  pos = 0;
  if (self->sjisFlag == 0) {
    for (;;) {
      glyph = UiFontNextGlyphSjis(text, &pos, &glyphW);
      if (glyph == -1) {
        return;
      }
      if (glyph == -2) {
        continue;
      }
      if (glyph == -3) {
        pen[0] = x;
        pen[1] = pen[1] + 13.0f;
        continue;
      }
      if (!(pen[0] <= 480.0f)) {
        return;
      }
      if (!(pen[0] <= -16.0f)) {
        page = glyph / 0x6e4;
        if (page >= 3) {
          fallbackPos = 0;
          glyph = UiFontNextGlyphSjis((char *)g_uiSjisFallbackGlyph, &fallbackPos, &glyphW);
          page = glyph / 0x6e4;
        }
        if (self->glyphsPerRow >= 2) {
          texture = self->fontTextures[0];
          sprite = GfxSpriteLayerCreateTemplateSprite(&self->layer, texture, pen);
          sprite->textureSlot = page;
        }
        else {
          texture = self->fontTextures[page];
          sprite = GfxSpriteLayerCreateTemplateSprite(&self->layer, texture, pen);
        }
        sprite->texture = texture;
        GfxSpriteSetSize(sprite, glyphW, 12.0f);
        sjisRect[0] = (float)(((glyph % 0x6e4) % 42) * 12);
        sjisRect[1] = (float)(((glyph % 0x6e4) / 42) * 12);
        sjisRect[2] = glyphW;
        sjisRect[3] = 12.0f;
        GfxSpriteSetUvRectXYWH(sprite, sjisRect);
      }
      pen[0] = pen[0] + glyphW;
    }
  }
  if (text[pos] == '\0') {
    return;
  }
  do {
    if (text[pos] == '\n') {
      pen[1] = pen[1] + self->lineHeight;
      pen[0] = x;
    }
    else {
      if (!(pen[0] <= 480.0f)) {
        return;
      }
      glyph = text[pos] - 0x20;
      if (!(pen[0] <= -16.0f)) {
        sprite = GfxSpriteLayerCreateSprite(&self->layer, self->fontTextures[0], pen, false);
        rect[0] = (float)(glyph % perRow) * self->cellW + 0.1f;
        rect[1] = (float)(glyph / perRow) * self->cellH;
        /* Compare chain as in the asm: only a 24 px cell gets the half-texel y nudge. */
        if (!(self->cellW == 12.0f) && !(self->cellW == 16.0f) && self->cellW == 24.0f) {
          rect[1] = rect[1] + 0.5f;
        }
        rect[2] = self->cellW;
        rect[3] = self->cellH;
        GfxSpriteSetUvRectXYWH(sprite, rect);
        GfxSpriteSetSize(sprite, self->cellW, self->cellH);
        /* vec4 copy (lv.q/sv.q) of outlineColor into tint[0..2] and alpha. */
        sprite->tint[0] = self->outlineColor[0];
        sprite->tint[1] = self->outlineColor[1];
        sprite->tint[2] = self->outlineColor[2];
        sprite->alpha = self->outlineColor[3];
      }
      if (self->widthTable == NULL) {
        pen[0] = pen[0] + self->advanceX;
      }
      else {
        advance = (((const float *)self->widthTable)[glyph] + self->spacing) * self->widthScale +
                  0.5f;
        pen[0] = pen[0] + (float)(int)floorf(advance);
      }
    }
    pos = pos + 1;
  } while (text[pos] != '\0');
}
