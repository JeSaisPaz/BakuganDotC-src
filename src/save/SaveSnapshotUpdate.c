// bdc 0x088c2a1c SaveSnapshotUpdate
#include "bdc.h"

/* Copies the current save data block (`SaveGetDataBlock`, size `SaveGetDataBlockSize()`) into the
   snapshot buffer `g_saveSnapshotBuffer` while the snapshot flag `g_saveSnapshotEnabled` is set. */

void SaveSnapshotUpdate(void)

{
  void *src;
  size_t n;

  if (g_saveSnapshotEnabled != '\0') {
    src = SaveGetDataBlock();
    n = SaveGetDataBlockSize();
    memcpy(g_saveSnapshotBuffer,src,n);
  }
  return;
}
