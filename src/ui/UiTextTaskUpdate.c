// bdc 0x089eb8ec UiTextTaskUpdate
#include "bdc.h"

/* Update method of the text task (`UiTextTask`) (vtable `0x08af56e4` slot 2): runs the message box
   (`UiMsgBoxUpdate`) and message window (`UiMsgWindowUpdate`) updates and, stepping `step`,
   creates the text printers once the font is ready: shared box, message box (font 1) and message
   window (font 1) (`UiTextBoxCreatePrinter`, `UiTextPrinterSetFont`). Each step advances only
   when its printer was created; steps outside 0..2 do nothing. */

void UiTextTaskUpdate(CoreTask *task)
{
  UiTextTask *self = (UiTextTask *)task;

  if (UiMsgBoxExists()) {
    UiMsgBoxUpdate((UiMsgBox *)UiMsgBoxGet());
  }
  if (UiMsgWindowExists()) {
    UiMsgWindowUpdate((UiMsgWindow *)UiMsgWindowGet());
  }
  switch (self->step) {
  case 0:
    if (UiTextBoxCreatePrinter((UiTextBox *)UiTextRenderGetBox(), 0) != 0) {
      self->step++;
    }
    break;
  case 1:
    if (UiTextBoxCreatePrinter(((UiMsgBox *)UiMsgBoxGet())->textBox, 0) != 0) {
      UiTextPrinterSetFont(UiTextBoxGetPrinter(((UiMsgBox *)UiMsgBoxGet())->textBox), 1);
      self->step++;
    }
    break;
  case 2:
    if (UiTextBoxCreatePrinter((UiTextBox *)((UiMsgWindow *)UiMsgWindowGet())->textBox, 0) != 0) {
      UiTextPrinterSetFont(UiTextBoxGetPrinter(((UiMsgWindow *)UiMsgWindowGet())->textBox), 1);
      self->step++;
    }
    break;
  }
}
