// bdc 0x088084c0 UiCopyrightTaskCtorArg
#include "bdc.h"

/* Constructor of the copyright-notice task (id 198) used by `CoreTaskNewByIdArg`: same as
   `UiCopyrightTaskCtor` but stores `arg` at `+0x24` instead of 0. */

UiCopyrightTask *UiCopyrightTaskCtorArg(UiCopyrightTask *task, u32 arg)
{
  bool fromLow;
  UiTextBox *box;
  UiTextBox *textBox;

  CoreTaskInit(&task->base);
  task->base.vtable = g_uiCopyrightTaskVtable;
  task->state = 0;
  task->step = 0;
  task->unk18 = 0;
  task->timer = 0;
  task->removeMe = 0;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  box = MemAlloc(sizeof(UiTextBox), (char *)0x0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  textBox = (UiTextBox *)0x0;
  if (box != (UiTextBox *)0x0) {
    UiTextBoxCtor(box);
    textBox = box;
  }
  task->textBox = textBox;
  textBox->packetDepth = 0.0f;
  task->unk24 = arg;
  return task;
}
