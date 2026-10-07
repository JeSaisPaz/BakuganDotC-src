// bdc 0x08935ef4 UiGauntletSetupShowFirstTimeHelp
#include "bdc.h"

/* Shows the one-time explanation of `UiGauntletSetup` in a confirm dialog: on
   the first call (`helpStep` = 0) sets `g_uiKeepSharedBg`, creates task 0x1fe (510, priority 100), sets
   its message to entry 35 of `"DWHologramHelp"` (`UiConfirmDialogSetMessage`) and advances
   `helpStep`; then waits until task 0x1fe no longer exists (`CoreTaskExists`). Returns 1 once
   done, else 0. */

s32 UiGauntletSetupShowFirstTimeHelp(UiGauntletSetup *self)

{
  UiConfirmDialog *task;
  u32 *table;

  if (self->helpStep == 0) {
    g_uiKeepSharedBg = 1;
    task = (UiConfirmDialog *)CoreTaskCreate(0x1fe,100);
    table = SaveFindLocalizedBin("DWHologramHelp");
    UiMesTableRelocate(table);
    UiConfirmDialogSetMessage(((char **)table)[0x23]);
    task->cursor = 0;
    task->unk84 = 1;
    self->helpStep = self->helpStep + 1;
  }
  else {
    if (1 < self->helpStep) {
      return 1;
    }
    g_uiKeepSharedBg = 1;
    if (CoreTaskExists(0x1fe) == 0) {
      self->helpStep = 2;
    }
  }
  return 0;
}
