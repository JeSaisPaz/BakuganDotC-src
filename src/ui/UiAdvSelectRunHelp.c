// bdc 0x08919b04 UiAdvSelectRunHelp
#include "bdc.h"

/* Runs the help dialog of the adventure partner-select screen (`UiAdvSelectCtor`, task 376):
   step 0 sets `g_uiKeepSharedBg`, opens the confirm dialog (task 0x1fe, priority 100) with the
   `DWMesHelp` line `UiButtonCellOffset(9, helpKind)` (single-button, `unk84` = 1, when `helpKind`
   is 0) and advances `helpStep`; step 1 waits until task 0x1fe is gone, then sets `helpCancelled`
   (1 only when `helpKind` is 1 and the answer was not yes) and moves to step 2. Returns 1 once
   `helpStep` is 2 or more, else 0. */

int UiAdvSelectRunHelp(UiAdvSelect *self)
{
  UiConfirmDialog *task;
  u32 *table;
  int line;

  if (self->helpStep == 0) {
    g_uiKeepSharedBg = 1;
    task = (UiConfirmDialog *)CoreTaskCreate(0x1fe, 100);
    table = SaveFindLocalizedBin("DWMesHelp");
    UiMesTableRelocate(table);
    line = UiButtonCellOffset(9, self->helpKind);
    UiConfirmDialogSetMessage((const char *)PspPtr(table[line]));
    task->cursor = 0;
    if (self->helpKind == 0) {
      task->unk84 = 1;
    }
    self->helpStep = self->helpStep + 1;
  }
  else {
    if (1 < self->helpStep) {
      return 1;
    }
    g_uiKeepSharedBg = 1;
    if (CoreTaskExists(0x1fe) == 0) {
      self->helpCancelled = 0;
      if (self->helpKind == 1) {
        if (UiConfirmDialogGetResult() == 1) {
          self->helpCancelled = 0;
        }
        else {
          self->helpCancelled = 1;
        }
      }
      self->helpStep = 2;
    }
  }
  return 0;
}
