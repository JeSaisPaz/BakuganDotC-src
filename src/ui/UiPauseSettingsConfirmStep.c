// bdc 0x089adfe4 UiPauseSettingsConfirmStep
#include "bdc.h"

/* Runs the confirm dialog of `UiPauseSettings` one frame. Step 0: sets
   `g_uiKeepSharedBg` = 1, creates the yes/no dialog task 510 (`CoreTaskCreate`,
   `UiConfirmDialogCtor`), loads the localized `DWMesHelp` table (`SaveFindLocalizedBin`,
   `UiMesTableRelocate`), sets entry `dlgMessage` as its text (`UiConfirmDialogSetMessage`),
   sets the dialog `cursor` to 1 and advances. Step 1: keeps g_uiKeepSharedBg set and, once task
   510 is gone, stores `dlgResult = 0` when `UiConfirmDialogGetResult` returned 1, else 1, and
   moves to step 2. Returns true only when called in step 2 or later (dialog closed), else false. */

bool UiPauseSettingsConfirmStep(UiPauseSettings *self)
{
  UiConfirmDialog *dialog;
  u32 *table;

  if (self->dlgStep == 0) {
    g_uiKeepSharedBg = 1;
    dialog = (UiConfirmDialog *)CoreTaskCreate(0x1fe, 100);
    table = SaveFindLocalizedBin("DWMesHelp");
    UiMesTableRelocate(table);
    UiConfirmDialogSetMessage(((const char **)table)[self->dlgMessage]);
    dialog->cursor = 1;
    self->dlgStep = self->dlgStep + 1;
  }
  else {
    if (self->dlgStep >= 2) {
      return true;
    }
    g_uiKeepSharedBg = 1;
    if (CoreTaskExists(0x1fe) == 0) {
      if (UiConfirmDialogGetResult() == 1) {
        self->dlgResult = 0;
      }
      else {
        self->dlgResult = 1;
      }
      self->dlgStep = 2;
    }
  }
  return false;
}
