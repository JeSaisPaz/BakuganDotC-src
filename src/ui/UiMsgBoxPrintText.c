// bdc 0x089ec520 UiMsgBoxPrintText
#include "bdc.h"

/* Prints the message box text `text` centred on the box (x `extents[0]`, y `extents[1] -
   extents[3] / 2`): first, when `shadow` is set, a shadow one pixel down-right in `colorB`, then
   the text in `colorA` (`UiTextBoxPrint`, proportional flag `proportional`). Each colour is
   copied into the printer's `outlineColor` and the printer's original alpha `outlineColor[3]` is put back before printing. */

void UiMsgBoxPrintText(UiMsgBox *self)
{
  s32 x;
  s32 y;
  float alpha;
  UiTextPrinter *printer;

  if (self->text != NULL) {
    x = (s32)self->extents[0];
    y = (s32)(self->extents[1] - self->extents[3] * 0.5f);
    printer = (UiTextPrinter *)UiTextBoxGetPrinter(self->textBox);
    alpha = printer->outlineColor[3];
    if (self->shadow != 0) {
      printer = (UiTextPrinter *)UiTextBoxGetPrinter(self->textBox);
      printer->outlineColor[0] = self->colorB[0];
      printer->outlineColor[1] = self->colorB[1];
      printer->outlineColor[2] = self->colorB[2];
      printer->outlineColor[3] = self->colorB[3];
      printer = (UiTextPrinter *)UiTextBoxGetPrinter(self->textBox);
      printer->outlineColor[3] = alpha;
      UiTextBoxPrint(self->textBox, x + 1, y + 1, self->text, self->proportional, 0);
    }
    printer = (UiTextPrinter *)UiTextBoxGetPrinter(self->textBox);
    printer->outlineColor[0] = self->colorA[0];
      printer->outlineColor[1] = self->colorA[1];
      printer->outlineColor[2] = self->colorA[2];
      printer->outlineColor[3] = self->colorA[3];
    printer = (UiTextPrinter *)UiTextBoxGetPrinter(self->textBox);
    printer->outlineColor[3] = alpha;
    UiTextBoxPrint(self->textBox, x, y, self->text, self->proportional, 0);
  }
}
