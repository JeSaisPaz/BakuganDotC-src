// bdc 0x088086bc UiCopyrightTaskDraw
#include "bdc.h"

/* Draw method of the copyright-notice task (vtable slot 4): while state `+0x10` is 0 and the text
   box at `+0x2c` has a text printer (`UiTextBoxHasPrinter`), draws it (`UiTextBoxDraw`). */

void UiCopyrightTaskDraw(UiCopyrightTask *task)
{
  if (task->textBox != (UiTextBox *)0x0 && task->state == 0 && UiTextBoxHasPrinter(task->textBox)) {
    UiTextBoxDraw(task->textBox);
  }
}
