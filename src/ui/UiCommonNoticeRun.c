// bdc 0x089a59bc UiCommonNoticeRun
#include "bdc.h"

/* Shows message 2 of `DWCommon_eu.bin` once in the confirm dialog (task 510,
   `UiConfirmDialogSetMessage`) and returns 1 after it has been closed; returns 0 while it is
   open. Progress is kept in `g_uiCommonNoticeState` (0 → open, 1 → waiting, 2 → done). */

int UiCommonNoticeRun(void)
{
  UiConfirmDialog *dialog;
  u32 *table;

  if (g_uiCommonNoticeState == 0) {
    g_uiKeepSharedBg = 1;
    dialog = (UiConfirmDialog *)CoreTaskCreate(0x1fe, 100);
    table = CorePackChainFind(g_ioLzsPackages, "DWCommon_eu.bin");
    UiMesTableRelocate(table);
    UiConfirmDialogSetMessage(((char **)table)[2]);
    dialog->cursor = 0;
    dialog->unk84 = 1;
    g_uiCommonNoticeState = g_uiCommonNoticeState + 1;
  }
  else {
    if (g_uiCommonNoticeState > 1) {
      return 1;
    }
    g_uiKeepSharedBg = 1;
    if (CoreTaskExists(0x1fe) == 0) {
      g_uiCommonNoticeState = 2;
    }
  }
  return 0;
}
