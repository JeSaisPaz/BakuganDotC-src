// bdc 0x089149e4 UiUpgradeRunConfirm
#include "bdc.h"

/* Runs the purchase confirmation of the Bakugan upgrade screen (`UiUpgradeCtor`, task 490),
   driven by `confirmStep`. Step 0 sets `g_uiKeepSharedBg`, opens the confirm dialog (task
   0x1fe, `CoreTaskCreate`) and sets its message: `kind` 0 shows `DMUpgrade` line 27 as a
   single-button notice, `kind` 1 formats language string 25 with the cost of the selected
   upgrade (`UiUpgradeGetUpgradeId` of `bakugan`/`focus`, `g_upgradeInfo`) through the
   message window and asks yes/no; other kinds open the dialog without a message. Step 1 sets
   `g_uiKeepSharedBg`, waits for the dialog task to end, stores yes (result 1) / no in
   `confirmChoice` and moves to step 2. Returns 1 once the step is >= 2 (or negative), else 0. */

int UiUpgradeRunConfirm(UiUpgrade *self, int kind)
{
  UiConfirmDialog *dialog;
  u32 *table;
  u8 upgradeId;
  char *format;
  char buf[512];

  if (self->confirmStep > 0) {
    if (self->confirmStep >= 2) {
      return 1;
    }
    g_uiKeepSharedBg = 1;
    if (CoreTaskExists(0x1fe) != 0) {
      return 0;
    }
    if (UiConfirmDialogGetResult() == 1) {
      self->confirmChoice = 1;
    }
    else {
      self->confirmChoice = 0;
    }
    self->confirmStep = 2;
    return 0;
  }
  if (self->confirmStep < 0) {
    return 1;
  }
  g_uiKeepSharedBg = 1;
  dialog = (UiConfirmDialog *)CoreTaskCreate(0x1fe, 100);
  table = SaveFindLocalizedBin("DMUpgrade");
  UiMesTableRelocate(table);
  if (kind == 0) {
    UiConfirmDialogSetMessage(((const char **)table)[27]);
    dialog->cursor = 0;
    dialog->unk84 = 1;
  }
  else if (kind == 1) {
    format = g_langStrings[25];
    upgradeId = UiUpgradeGetUpgradeId(self->bakugan, self->focus);
    sprintf(buf, format, (int)g_upgradeInfo[upgradeId].cost);
    UiMsgWindowPrintfUtf8(UiMsgWindowGet(), buf);
    UiConfirmDialogSetMessage(((UiMsgWindow *)UiMsgWindowGet())->text);
    dialog->cursor = 1;
    dialog->unk84 = 0;
  }
  self->confirmStep = self->confirmStep + 1;
  return 0;
}
