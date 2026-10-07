// bdc 0x088c2a70 SaveSnapshotBegin
#include "bdc.h"

/* Enables the save data snapshot (`g_saveSnapshotEnabled = 1`) and takes it
   (`SaveSnapshotUpdate`). Caller: `UiMainMenuApplySelection` (game start). */

void SaveSnapshotBegin(void)

{
  g_saveSnapshotEnabled = '\x01';
  SaveSnapshotUpdate();
  return;
}

