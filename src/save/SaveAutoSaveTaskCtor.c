// bdc 0x0880b834 SaveAutoSaveTaskCtor
#include "bdc.h"

/* Constructor of the autosave task (task id 10022 = 0x2726, 0x38 bytes, vtable
   `g_saveAutoSaveTaskVtbl`): clears the handler, clears profile word 1 when a profile exists,
   clears the step, sets `hasSlot = 1`, allocates a `UiTextBoxCtor` text box at `+0x24` (depth
   61000.0) and a 0x70-byte `GfxScreenFaderCtor` fader at `+0x28` from the low end of the heap,
   makes sure the message window exists and puts its depth 1000 above the active fader. Returns
   `task`. */

CoreTask *SaveAutoSaveTaskCtor(CoreTask *task)

{
  SaveAutoSaveTask *self = (SaveAutoSaveTask *)task;
  bool fromLow;
  UiTextBox *box;
  GfxScreenFader *fader;
  UiMsgWindow *window;

  CoreTaskInit(task);
  task->vtable = &g_saveAutoSaveTaskVtbl;
  self->handler = NULL;
  if (SaveHasProfile()) {
    SaveProfileSetWord(SaveGetProfile(), 1, 0);
  }
  self->step = 0;
  self->hasSlot = 1;
  self->unk21 = 0;
  self->timer = 0;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  box = MemAlloc(sizeof(UiTextBox), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (box != NULL) {
    UiTextBoxCtor(box);
  }
  self->textBox = box;
  box->packetDepth = 61000.0f; /* no NULL check in the original */
  self->unk30 = 0;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  fader = MemAlloc(sizeof(GfxScreenFader), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (fader != NULL) {
    GfxScreenFaderCtor(fader);
  }
  self->fader = (GfxFader *)fader;

  if (!UiMsgWindowExists()) {
    UiMsgWindowEnsure();
  }
  window = (UiMsgWindow *)UiMsgWindowGet();
  window->depth = GfxGetActiveFader()->sortKey + 1000.0f;
  return task;
}
