// bdc 0x088c2a94 SaveSnapshotClear
#include "bdc.h"

/* Clears the save snapshot flag `g_saveSnapshotEnabled`. */

void SaveSnapshotClear(void)

{
  g_saveSnapshotEnabled = '\0';
  return;
}

