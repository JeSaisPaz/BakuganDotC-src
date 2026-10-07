// bdc 0x08816510 UiMsgWindowSetText
#include "bdc.h"

/* Sets the message window's text: measures `text` (already encoded) with the text box's printer
   (`UiTextMeasure`) and copies it into the window's text buffer (`+0xc`). Returns 1, or 0 when
   `text` is NULL or the printer does not exist yet. */

s32 UiMsgWindowSetText(UiMsgWindow *self, u8 *text)
{
  void *printer;
  s32 ok;
  float width;
  float height;
  int length;

  ok = 0;
  if (text != (u8 *)0x0) {
    printer = UiTextBoxGetPrinter(self->textBox);
    width = 0.0f;
    height = 0.0f;
    length = 0;
    if (printer != (void *)0x0) {
      UiTextMeasure(0.0f, printer, (char *)text, &width, &height, &length);
      memcpy(self->text, text, length);
      ok = 1;
    }
  }
  return ok;
}
