// bdc 0x0890477c BtlStageCamSceneUnload
#include "bdc.h"

/* Frees a stage camera scene's file buffer, then for each of the 8 slot indices the key array of
   the head slot and of the same slot in the five further blocks (in that order), each under
   `MemLock`, and clears every freed pointer. NULL pointers are skipped. */
void BtlStageCamSceneUnload(BtlStageCamScene *scene)
{
  int i;
  int b;

  if (scene->fileBuf != NULL) {
    MemLock();
    MemFree(scene->fileBuf, NULL, 0);
    MemUnlock();
    scene->fileBuf = NULL;
  }
  for (i = 0; i < 8; i++) {
    if (scene->head[i].keys != NULL) {
      MemLock();
      MemFree(scene->head[i].keys, NULL, 0);
      MemUnlock();
      scene->head[i].keys = NULL;
    }
    for (b = 0; b < 5; b++) {
      if (scene->blocks[b][i].keys != NULL) {
        MemLock();
        MemFree(scene->blocks[b][i].keys, NULL, 0);
        MemUnlock();
        scene->blocks[b][i].keys = NULL;
      }
    }
  }
}
