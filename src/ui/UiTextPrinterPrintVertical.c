// bdc 0x08819528 UiTextPrinterPrintVertical
#include "bdc.h"

/* Text printer vtable slot 3 (`0x08af1654`): prints the game-encoded
   string `text` top-to-bottom starting at (`x`, `y`): each glyph from `UiFontNextGlyph` becomes a
   sprite (`GfxSpriteLayerCreateTemplateSprite`) cut from the font texture page `glyph /
   glyphsPerPage` (cell size `cellW`×`cellH` in 512×512 pages, scale `scale`), tinted with the
   current colour `outlineColor`; y advances by `lineHeight` per glyph, a newline (-3/-4) moves x
   by `advanceX` and resets y. Returns when the main text ends (-1). */

void UiTextPrinterPrintVertical(float x, float y, UiTextPrinter *self, const char *text)

{
  float pos[4];
  int textPos;
  int insertPos;
  float rect[4];
  float rectScaled[4];
  float startY;
  int glyph;
  u32 cellsPerRow;
  u32 cellsPerCol;
  int perPage;
  int cell;
  void *texture;
  GfxSprite *sprite;
  float u;
  float v;

  pos[0] = x;
  pos[1] = y;
  pos[2] = 0.0f;
  pos[3] = 0.0f;
  textPos = 0;
  insertPos = 0;
  startY = y;
  for (;;) {
    if (self->inInsert != 0) {
      glyph = UiFontNextGlyph((char *)self->insertText, &insertPos);
      if (glyph == -1) {
        self->inInsert = 0;
        self->ctrlF0Flag = 0;
        continue;
      }
    }
    else {
      glyph = UiFontNextGlyph((char *)text, &textPos);
      if (glyph == -1) {
        return;
      }
    }
    if (glyph == -2) {
      continue;
    }
    if (glyph == -3 || glyph == -4) {
      /* newline: next column */
      pos[0] = pos[0] + self->advanceX;
      pos[1] = startY;
      if (self->inInsert != 0) {
        self->inInsert = 0;
        self->ctrlF0Flag = 0;
      }
      continue;
    }
    if (glyph == -0x11) {
      pos[1] = pos[1] + self->lineHeight;
      continue;
    }
    if (glyph == -5) {
      pos[1] = pos[1] + self->lineHeight * 0.5f;
      continue;
    }
    if (glyph < -5 && glyph >= -0xb) {
      /* colour codes: only rgb is written, alpha is kept */
      switch (glyph) {
      case -0xb:
        self->outlineColor[0] = 0.0f;
        self->outlineColor[1] = 0.0f;
        self->outlineColor[2] = 1.0f;
        break;
      case -0xa:
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
      default: /* -6: back to the default colour (16-byte copy) */
        self->outlineColor[0] = self->color[0];
        self->outlineColor[1] = self->color[1];
        self->outlineColor[2] = self->color[2];
        self->outlineColor[3] = self->color[3];
        break;
      }
      continue;
    }
    if (glyph == -0x10) {
      self->ctrlF0Flag = 1;
      continue;
    }
    if (glyph < 0) {
      /* other control codes are ignored */
      continue;
    }

    cellsPerRow = 0x200 / (u32)self->cellW;
    cellsPerCol = 0x200 / (u32)self->cellH;
    perPage = (int)(cellsPerRow * cellsPerCol);
    texture = self->fontTextures[glyph / perPage];
    sprite = GfxSpriteLayerCreateTemplateSprite(&self->layer, texture, pos);
    cell = glyph % perPage;
    u = (float)((u32)(cell % (int)cellsPerRow) * (u32)self->cellW);
    v = (float)((u32)(cell / (int)cellsPerCol) * (u32)self->cellH);
    sprite->texture = texture;
    if (self->scale == 1.0f) {
      GfxSpriteSetSize(sprite, self->cellW, self->cellH);
      rect[0] = u;
      rect[1] = v;
      rect[2] = self->cellW;
      rect[3] = self->cellH;
      GfxSpriteSetUvRectXYWH(sprite, rect);
    }
    else {
      sprite->flags = sprite->flags | 0x20;
      GfxSpriteSetSize(sprite, self->cellW * self->scale, self->cellH * self->scale);
      rectScaled[0] = u + 1.0f;
      rectScaled[1] = v + 1.0f;
      rectScaled[2] = self->cellW - 1.0f;
      rectScaled[3] = self->cellH;
      GfxSpriteSetUvRectXYWH(sprite, rectScaled);
    }
    sprite->textureSlot = (cell > 0x205) ? 1 : 0;
    /* outlineColor -> tint[0..2] + alpha (16 bytes at +0xb0) */
    sprite->tint[0] = self->outlineColor[0];
    sprite->tint[1] = self->outlineColor[1];
    sprite->tint[2] = self->outlineColor[2];
    sprite->alpha = self->outlineColor[3];
    if (self->scale == 1.0f) {
      pos[1] = pos[1] + self->lineHeight;
    }
    else {
      pos[1] = pos[1] + self->lineHeight * self->scale;
    }
  }
}
