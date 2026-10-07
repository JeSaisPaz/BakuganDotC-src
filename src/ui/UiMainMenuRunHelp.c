// bdc 0x089a95a4 UiMainMenuRunHelp
#include "bdc.h"

/* Shows the first-time help messages of the main menu. Step 0 (`helpStep`): sets
   `g_uiKeepSharedBg`, opens the confirm dialog (task 510) with message `helpMsg` of `DWMesHelp`
   (messages below 4 set the dialog's `cursor` to 1, the others its `unk84`), advances the step and
   returns 0. Step 1: sets `g_uiKeepSharedBg` and, once the dialog task is gone, stores the answer
   in `helpResult` (0 = yes, 1 = otherwise), moves to step 2 and returns 0. Step 2+: when profile
   `playthroughClearBits` bit 0 is set, continues with the next message while `helpMsg` < 6
   (returns 0), else clears bit 0, sets `helpResult` to 1 and returns 1; otherwise when bit 1 is
   set clears it and sets `helpResult` to 1; returns 1. */

int UiMainMenuRunHelp(UiMainMenu *self)
{
  UiConfirmDialog *dialog;
  u32 *table;
  SaveProfileData *data;

  if (self->helpStep == 0) {
    g_uiKeepSharedBg = 1;
    dialog = (UiConfirmDialog *)CoreTaskCreate(0x1fe, 100);
    table = SaveFindLocalizedBin("DWMesHelp");
    UiMesTableRelocate(table);
    UiConfirmDialogSetMessage(((char **)table)[self->helpMsg]);
    if (self->helpMsg < 4) {
      dialog->cursor = 1;
    } else {
      dialog->unk84 = 1;
    }
    self->helpStep = self->helpStep + 1;
    return 0;
  }
  if (self->helpStep < 2) {
    g_uiKeepSharedBg = 1;
    if (CoreTaskExists(0x1fe) == 0) {
      if (UiConfirmDialogGetResult() == 1) {
        self->helpResult = 0;
      } else {
        self->helpResult = 1;
      }
      self->helpStep = 2;
    }
    return 0;
  }
  if ((SaveGetProfile()->data->playthroughClearBits & 1) != 0) {
    if (self->helpMsg < 6) {
      UiMainMenuStartHelp(self, self->helpMsg + 1);
      return 0;
    }
    data = SaveGetProfile()->data;
    data->playthroughClearBits = data->playthroughClearBits & ~1;
    self->helpResult = 1;
  } else if ((SaveGetProfile()->data->playthroughClearBits & 2) != 0) {
    data = SaveGetProfile()->data;
    data->playthroughClearBits = data->playthroughClearBits & ~2;
    self->helpResult = 1;
  }
  return 1;
}
