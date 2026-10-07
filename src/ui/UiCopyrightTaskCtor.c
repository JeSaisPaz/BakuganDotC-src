// bdc 0x088083fc UiCopyrightTaskCtor
#include "bdc.h"

/* Constructor of the copyright/legal-notice task (task id 198 = 0xc6, 0x30 bytes, vtable
   `0x08af13ec`, built by `CoreTaskNewById`): runs `CoreTaskInit`, clears the state words and
   allocates the 0x10-byte text-window helper at `+0x2c` (`UiTextBoxCtor`). The task fades in the
   language's Activision/Bakugan copyright text, holds it for three seconds and fades out. */

UiCopyrightTask *UiCopyrightTaskCtor(UiCopyrightTask *task)
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
  task->unk24 = 0;
  return task;
}
