// bdc 0x089ec630 UiMsgBoxUpdateCursor
#include "bdc.h"

/* Positions the selection highlight of a message box with choices: when the box has text (`+0x18`)
   and a choice table (`+0x5c`, 8-byte entries `{float x, s16 line, s16 width}`), places the
   highlight rect (`+0x7c`) over choice `+0x60` (hidden when out of range `0..+0x64`) using the text
   printer's glyph size (`UiTextBoxGetPrinter` `+0x94`/`+0x98`) and the box centre/size
   (`+0x20..+0x2c`), or `UiMsgBoxChoiceX` for proportional layout (`+0x5a`), sizes it with
   `GfxRectSetSize`, and ping-pongs its blink alpha `+0x68` by the step `+0x6c` (5x faster while
   `+0x1c > 0`). Called by `UiMsgBoxUpdate`. */

void UiMsgBoxUpdateCursor(UiMsgBox *self)
{
  GfxRect *rect;
  UiTextPrinter *printer;
  s32 index;
  s32 left;
  s32 top;
  s32 width;
  float alpha;
  float step;

  if (self->text == NULL || self->choices == NULL) {
    return;
  }
  left = (s32)(self->extents[0] - self->extents[2] * 0.5f);
  top = (s32)(self->extents[1] - self->extents[3] * 0.5f);
  rect = self->highlight;
  index = self->choice;
  if (index < 0 || index >= (s32)self->unk64) {
    GfxRectSetVisible(rect, 0);
    return;
  }
  if (self->proportional != 0) {
    rect->pos[0] = (float)UiMsgBoxChoiceX(self, index) - 2.0f;
  }
  else {
    printer = UiTextBoxGetPrinter(self->textBox);
    rect->pos[0] = ((float)left + printer->advanceX * self->choices[self->choice].x) - 1.0f;
  }
  printer = UiTextBoxGetPrinter(self->textBox);
  rect->pos[1] = (float)top + printer->lineHeight * (float)self->choices[self->choice].line;
  width = (s32)((float)self->choices[self->choice].width + 2.0f);
  printer = UiTextBoxGetPrinter(self->textBox);
  GfxRectSetSize(rect, width, (s32)printer->lineHeight);

  step = self->unk6c;
  alpha = self->unk68 + step;
  self->unk68 = alpha;
  if ((s32)self->unk1c > 0) {
    alpha = alpha + step * 5.0f;
    self->unk68 = alpha;
  }
  if (!(alpha <= 1.0f)) {
    self->unk68 = 1.0f;
    self->unk6c = -step;
    alpha = 1.0f;
  }
  else if (alpha < 0.0f) {
    self->unk68 = 0.0f;
    self->unk6c = -step;
    alpha = 0.0f;
  }
  rect->color[3] = alpha * 0.5f + 0.5f;
  GfxRectSetVisible(rect, 1);
}
