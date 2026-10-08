// bdc 0x08929b54 UiHologramViewRunHelp
#include "bdc.h"

/* Runs the help dialog of the hologram detail view (`UiHologramViewCtor`, task 392): step 0 sets
   g_uiKeepSharedBg, creates the confirm dialog (task 0x1fe, priority 100) with `DWHologramHelp`
   line `page`, clears its cursor (and unk91 for view kinds 1 and 7), sets unk84 = 1 and advances;
   step 1 keeps g_uiKeepSharedBg set and moves to step 2 once task 0x1fe is gone. Returns 1 once
   helpStep >= 2, else 0. */

int UiHologramViewRunHelp(UiHologramView *self)

{
  UiConfirmDialog *task;
  u32 *table;
  u8 kind;

  if (self->helpStep != 0) {
    if (self->helpStep >= 2) {
      return 1;
    }
    g_uiKeepSharedBg = 1;
    if (CoreTaskExists(0x1fe) == 0) {
      self->helpStep = 2;
    }
    return 0;
  }
  g_uiKeepSharedBg = 1;
  task = (UiConfirmDialog *)CoreTaskCreate(0x1fe,100);
  table = SaveFindLocalizedBin("DWHologramHelp");
  UiMesTableRelocate(table);
  UiConfirmDialogSetMessage((const char *)PspPtr(table[self->page]));
  kind = self->kind;
  if (kind == 1 || kind == 7) {
    task->unk91 = 0;
  }
  task->cursor = 0;
  task->unk84 = 1;
  self->helpStep = self->helpStep + 1;
  return 0;
}
