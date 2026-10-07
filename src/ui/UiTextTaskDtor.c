// bdc 0x089eb7dc UiTextTaskDtor
#include "bdc.h"

/* Destructor of the text task (id 10050, vtable `0x08af56e4` slot 1): destroys the message window
   (`UiMsgWindowDestroy`), the message box (`UiMsgBoxDestroy`) and the shared text renderer
   (`UiTextRenderDestroy`), then `CoreTaskDestroy`; frees itself when `flags & 1`. */

void UiTextTaskDtor(CoreTask *task, u32 flags)

{
  if (task != (CoreTask *)0x0) {
    task->vtable = g_uiTextTaskVtbl;
    UiMsgWindowDestroy();
    UiMsgBoxDestroy();
    UiTextRenderDestroy();
    CoreTaskDestroy(task,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(task,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

