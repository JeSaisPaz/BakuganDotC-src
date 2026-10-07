// bdc 0x088cb9ec UiTalkBalloonMeasureText
#include "bdc.h"

/* Measures the message text `textCursor` of the talk balloon (`UiTalkBalloonCtor`) without
   drawing, walking the glyphs with `UiFontNextGlyph`. Each line starts 24 px (+16 indent once an
   icon line ended) wide; glyphs add `glyphWidth` (code -5 half of it; -2, -18 and -6..-10 nothing;
   -11/-12 skip one extra byte). -3 is a line break (`lineHeight` down); with a `lineCount` limit the
   break on the last line becomes -4, a page break: with `pageFlags[0]` it acts as a line break, else it
   restarts the height at 0 (or `lineHeight + 4` with `pageFlags[2]` once an icon was seen) and also
   counts one glyph width. -16 marks an icon line (+4 height at its end with `pageFlags[2]`, then
   16 px indent) and makes the result 1; -13..-15 and -19 switch to a secondary text pointer (which
   starts NULL) until its -1. The main text's -1 ends: `outSize` = {widest line, tallest height, 0, 0}.
   Returns 1 if an icon code (-16) was met, else 0. */

u8 UiTalkBalloonMeasureText(UiTalkBalloon *self, float *outSize)

{
  char *text;
  char *subText;
  int pos;
  int c;
  int lineIndex;
  bool inSub;
  bool iconLine;
  bool sawIcon;
  u8 result;
  float indent;
  float width;
  float height;
  float maxWidth;
  float maxHeight;
  float gw;

  text = self->textCursor;
  result = 0;
  indent = 0.0f;
  maxWidth = 0.0f;
  maxHeight = 0.0f;
  height = 0.0f;
  width = 24.0f;
  iconLine = false;
  inSub = false;
  sawIcon = false;
  subText = (char *)0x0;
  lineIndex = 0;
  for (;;) {
    if (maxWidth < width) {
      maxWidth = width;
    }
    if (maxHeight < height) {
      maxHeight = height;
    }
    pos = 0;
    if (inSub) {
      c = UiFontNextGlyph(subText, &pos);
      subText = subText + pos;
    }
    else {
      c = UiFontNextGlyph(text, &pos);
      text = text + pos;
    }
    if (c == -12 || c == -11) {
      text = text + 1;
      continue;
    }
    if (self->lineCount > 0 && c == -3 && !(lineIndex + 1 < self->lineCount)) {
      c = -4;
    }
    if (c < -5 && !(c < -10)) {
      continue;
    }
    if (c == -1) {
      if (!inSub) {
        break;
      }
      inSub = false;
      if (iconLine) {
        iconLine = false;
        if (self->pageFlags[2] != 0) {
          height = height + 4.0f;
        }
        indent = 16.0f;
      }
      continue;
    }
    if (c == -3) {
      inSub = false;
      if (iconLine) {
        iconLine = false;
        if (self->pageFlags[2] != 0) {
          height = height + 4.0f;
        }
        indent = 16.0f;
      }
      width = indent + 24.0f;
      lineIndex = lineIndex + 1;
      height = height + self->lineHeight;
      continue;
    }
    if (c == -4) {
      lineIndex = 0;
      if (self->pageFlags[0] != 0) {
        width = indent + 24.0f;
        height = height + self->lineHeight;
        continue;
      }
      height = 0.0f;
      width = indent + 24.0f;
      if (self->pageFlags[2] != 0 && sawIcon) {
        height = self->lineHeight + 4.0f + 0.0f;
      }
    }
    else {
      if ((c < -12 && !(c < -15)) || c == -19) {
        inSub = true;
        continue;
      }
      if (c == -16) {
        iconLine = true;
        sawIcon = true;
        result = 1;
        continue;
      }
      if (c == -18) {
        continue;
      }
    }
    if (c == -2) {
      continue;
    }
    gw = self->glyphWidth;
    if (c == -17) {
      width = width + gw;
    }
    else if (c == -5) {
      width = width + gw * 0.5f;
    }
    else {
      width = width + gw;
    }
  }
  if (maxWidth < width) {
    maxWidth = width;
  }
  if (maxHeight < height) {
    maxHeight = height;
  }
  outSize[0] = maxWidth;
  outSize[1] = maxHeight;
  outSize[2] = 0.0f;
  outSize[3] = 0.0f;
  return result;
}
