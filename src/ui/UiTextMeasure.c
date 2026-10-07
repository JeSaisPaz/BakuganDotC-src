// bdc 0x088184a8 UiTextMeasure
#include "bdc.h"

/* Measures `text` as the printer `printer` would lay it out: decodes glyphs with
   `UiFontNextGlyph`, applies the same word-wrap rule (`UiTextCanBreakBefore`, a word wraps when
   it would end past `wrapWidth - margin`) and glyph advances (`UiFontGlyphAdvance`). Stores the
   total height in `*outH`, the widest line in `*outW` and the byte length including the terminator
   in `*outLen` (each when not NULL), and returns the number of printable glyphs. Also accumulates
   `g_uiTextMeasureBreakWidth`. Empty strings return 0 and leave the outputs untouched. */

/* vf2id.s/vi2f.s pair through S000: floor(x) as an int, back to float. The inputs are glyph
   advances and line widths, far inside the int range and never NaN. */
static inline float UiTextMeasureFloor(float x)
{
  return (float)(int)floorf(x);
}

int UiTextMeasure(float margin, void *printer, char *text, float *outH, float *outW, int *outLen)

{
  struct UiTextPrinter *p = (struct UiTextPrinter *)printer;
  int pos;
  int peek;
  int wordPos;
  int wordPeek;
  int count;
  int glyph;
  int g;
  int wordGlyphs;
  float lineW;
  float width;
  float height;
  float wordW;
  float scale;

  if (strcmp(text, "") == 0) {
    return 0;
  }
  height = 0.0f;
  g_uiTextMeasureBreakWidth = 0.0f;
  count = 0;
  lineW = 0.0f;
  width = 0.0f;
  pos = 0;
  for (;;) {
    glyph = UiFontNextGlyph(text, &pos);
    if (glyph == -1) {
      break;
    }
    peek = pos;
    if (UiTextCanBreakBefore(glyph, UiFontNextGlyph(text, &peek)) != 0) {
      /* break opportunity: measure the following word */
      wordGlyphs = 0;
      wordPos = pos;
      wordW = lineW + UiFontGlyphAdvance(printer, 0);
      for (;;) {
        g = UiFontNextGlyph(text, &wordPos);
        if (g <= 0) {
          /* French: a space (-5) before a glyph that cannot start a line belongs to the word */
          if (!SaveLanguageIsFrench() || g != -5) {
            break;
          }
          wordPeek = wordPos;
          if (UiTextCanBreakBefore(g, UiFontNextGlyph(text, &wordPeek)) != 0) {
            break;
          }
        }
        wordGlyphs++;
        if (g == -5) {
          wordW = wordW + UiFontGlyphAdvance(printer, 0);
        } else {
          wordW = wordW + UiTextMeasureFloor(UiFontGlyphAdvance(printer, g));
        }
      }
      if (wordGlyphs > 0 && (int)(p->wrapWidth - margin) < (int)wordW) {
        glyph = -3;
      }
      if (!(lineW <= width)) {
        width = lineW;
      }
      if (p->widthTable != 0) {
        g_uiTextMeasureBreakWidth =
            g_uiTextMeasureBreakWidth + UiTextMeasureFloor(UiFontGlyphAdvance(printer, glyph));
      }
    }
    if (glyph == -2) {
      continue;
    }
    if (glyph == -3 || glyph == -4) {
      /* line break */
      height = height + p->lineHeight;
      lineW = 0.0f;
    } else if (glyph == -0x11) {
      lineW = lineW + p->advanceX;
      if (!(lineW <= width)) {
        width = lineW;
      }
    } else if (glyph == -5) {
      /* space */
      if (p->widthTable != 0) {
        lineW = lineW + UiFontGlyphAdvance(printer, 0);
      } else {
        lineW = lineW + p->advanceX * 0.5f;
      }
      if (!(lineW <= width)) {
        width = lineW;
      }
    } else if (glyph >= 0) {
      /* every other negative code is a control code that takes no room */
      count++;
      scale = p->scale;
      if (height == 0.0f) {
        height = height + p->lineHeight;
      }
      if (scale == 1.0f) {
        lineW = UiTextMeasureFloor(lineW);
        lineW = lineW + UiTextMeasureFloor(UiFontGlyphAdvance(printer, glyph));
      } else {
        lineW = lineW + UiFontGlyphAdvance(printer, glyph);
      }
      if (!(lineW <= width)) {
        width = lineW;
      }
    }
  }
  if (outH != NULL) {
    *outH = height;
  }
  if (outW != NULL) {
    *outW = width;
  }
  if (outLen != NULL) {
    *outLen = pos + 1;
  }
  return count;
}
