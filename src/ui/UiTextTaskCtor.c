// bdc 0x089eb73c UiTextTaskCtor
#include "bdc.h"

/* Constructor of the text task (task id 10050 / 0x2742, object size 0x14, vtable `0x08af56e4`)
   created by `CoreTaskNewById`: base `CoreTaskInit`, stores itself in `g_uiTextTask`, and
   makes sure the shared text renderers exist: the text box
   (`UiTextRenderExists`/`UiTextRenderEnsure`, depth 20000.0 via `UiTextRenderGetBox`), the
   message box (`UiMsgBoxExists`/`UiMsgBoxEnsure`) and the message window
   (`UiMsgWindowExists`/`UiMsgWindowEnsure`). */

CoreTask *UiTextTaskCtor(CoreTask *task)

{
  UiTextTask *self = (UiTextTask *)task;

  CoreTaskInit(task);
  task->vtable = g_uiTextTaskVtbl;
  g_uiTextTask = task;
  if (!UiTextRenderExists()) {
    UiTextRenderEnsure();
    *(float *)UiTextRenderGetBox() = 20000.0f;
  }
  if (!UiMsgBoxExists()) {
    UiMsgBoxEnsure();
  }
  if (!UiMsgWindowExists()) {
    UiMsgWindowEnsure();
  }
  self->step = 0;
  return task;
}
