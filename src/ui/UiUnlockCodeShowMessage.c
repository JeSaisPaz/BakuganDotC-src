// bdc 0x08992f10 UiUnlockCodeShowMessage
#include "bdc.h"

/* Runs the message dialog of `UiUnlockCode`: on the first call creates the
   confirm-dialog task 0x1fe (`CoreTaskCreate`, priority 100), sets its text to message `+0x10d`
   of `"DWSpecialUnlock"` (`SaveFindLocalizedBin`, `UiConfirmDialogSetMessage`) as a plain
   notice; then waits until the task is gone. Returns 1 once the dialog has closed. */

int UiUnlockCodeShowMessage(UiUnlockCode *self)

{
  UiConfirmDialog *dialog;
  char **table;
  void *raw;
  s32 exists;
  
  if (self->messageStep == 0) {
    g_uiKeepSharedBg = 1;
    dialog = (UiConfirmDialog *) CoreTaskCreate(0x1fe,100);
    raw = SaveFindLocalizedBin("DWSpecialUnlock");
    table = raw;
    UiMesTableRelocate(raw);
    UiConfirmDialogSetMessage(table[self->messageId]);
    dialog->cursor = 0;
    dialog->unk84 = 1;
    self->messageStep = self->messageStep + 1;
  }
  else {
    if (1 < self->messageStep) {
      return 1;
    }
    g_uiKeepSharedBg = 1;
    exists = CoreTaskExists(0x1fe);
    if (exists == 0) {
      self->messageStep = 2;
    }
  }
  return 0;
}

