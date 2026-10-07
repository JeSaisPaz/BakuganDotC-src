// bdc 0x08808590 UiCopyrightTaskDtor
#include "bdc.h"

/* Destructor of the copyright-notice task (vtable slot 1): waits for the GE (`GfxWaitGeIdle`),
   deletes the text box at `+0x2c` (`UiTextBoxDelete`), chains to `CoreTaskDestroy` and frees
   the object when bit 0 of `flags` is set. */

void UiCopyrightTaskDtor(UiCopyrightTask *task, u32 flags)
{
  if (task != (UiCopyrightTask *)0x0) {
    task->base.vtable = g_uiCopyrightTaskVtable;
    GfxWaitGeIdle();
    if (task->textBox != (UiTextBox *)0x0) {
      UiTextBoxDelete(task->textBox, 3);
      task->textBox = (UiTextBox *)0x0;
    }
    CoreTaskDestroy(&task->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(task, (char *)0x0, 0);
      MemUnlock();
    }
  }
}
