// bdc 0x0899be3c UiWorldMapShowConfirm
#include "bdc.h"

/* Runs the yes/no confirm dialog of `UiWorldMap` (step in `dialogState[1]`): after one
   idle frame creates the dialog task 0x1fe (`CoreTaskCreate`) with a `"DWMesHelp"` message (index
   from `UiButtonCellOffset(3, 0)`), waits for it to close and stores the answer in `dialogState[0]`
   (0 = yes, 1 = no; `UiConfirmDialogGetResult`). While the dialog is open and profile flag 0 and
   profile flags 0x80 are set it removes the dialog and answers no with a quick fade. Steps 3 and 4
   are idle frames; returns 0 while running and 1 once the step reaches 5 (clearing `confirmActive`). */

int UiWorldMapShowConfirm(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  u8 step;
  UiConfirmDialog *dialog;
  u32 *table;
  GfxFader *fader;

  step = map->dialogState[1];
  if (step >= 5) {
    map->confirmActive = 0;
    return 1;
  }
  if (step == 1) {
    g_uiKeepSharedBg = 1;
    dialog = (UiConfirmDialog *)CoreTaskCreate(0x1fe, 100);
    table = SaveFindLocalizedBin("DWMesHelp");
    UiMesTableRelocate(table);
    UiConfirmDialogSetMessage(((char **)table)[UiButtonCellOffset(3, 0)]);
    dialog->cursor = 0;
    map->dialogState[1] = map->dialogState[1] + 1;
  } else if (step == 2) {
    if (CoreTaskExists(0x1fe) == 0) {
      if (UiConfirmDialogGetResult() == 1) {
        map->dialogState[0] = 0;
      } else {
        map->dialogState[0] = 1;
      }
      map->dialogState[1] = 3;
    } else if (SaveGetProfileFlag0() != 0 && SaveHasProfile() &&
               SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
      CoreTaskRemoveById(0x1fe);
      map->dialogState[0] = 1;
      map->dialogState[1] = 3;
      fader = GfxGetActiveFader();
      fader->end[0] = 0.0f;
      fader->end[1] = 0.0f;
      fader->end[2] = 0.0f;
      fader->end[3] = 0.0f;
      GfxFaderSetPreset(GfxGetActiveFader(), 3);
      GfxFaderStart(GfxGetActiveFader(), 3);
    }
    g_uiKeepSharedBg = 1;
  } else if (step == 3) {
    map->dialogState[1] = 4;
  } else if (step == 4) {
    map->dialogState[1] = 5;
  } else {
    map->dialogState[1] = step + 1;
  }
  return 0;
}
