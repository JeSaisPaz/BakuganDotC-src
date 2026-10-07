// bdc 0x08808940 UiCopyrightTaskStateEnd
#include "bdc.h"

/* State 1 of the copyright-notice task: hides the text box's printer if any (`UiTextBoxClear`) and
   sets the remove-me byte `+0x28`, so `UiCopyrightTaskUpdate` deletes the task. */

void UiCopyrightTaskStateEnd(UiCopyrightTask *task)
{
  if (task->textBox != (UiTextBox *)0x0) {
    UiTextBoxClear(task->textBox);
  }
  task->removeMe = 1;
}
