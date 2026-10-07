// bdc 0x08904f38 BtlDemoSceneUnload
#include "bdc.h"

/* Frees what a `.scb` demo scene loaded: the file buffer, then for each of the 8 slot indices the
   key array of `track0[i]` and of `tracks[0..4][i]` (in that order), each under `MemLock` with
   the freed pointer cleared and NULL pointers skipped; finally deletes the scene objects
   (`BtlDemoSceneDeleteObjects`) and clears the event tables (`BtlDemoSceneEventListClear`). */
void BtlDemoSceneUnload(BtlDemoScene *scene)
{
  int i;
  int t;

  if (scene->buffer != NULL) {
    MemLock();
    MemFree(scene->buffer, NULL, 0);
    MemUnlock();
    scene->buffer = NULL;
  }
  for (i = 0; i < 8; i++) {
    if (scene->track0[i].keys != NULL) {
      MemLock();
      MemFree(scene->track0[i].keys, NULL, 0);
      MemUnlock();
      scene->track0[i].keys = NULL;
    }
    for (t = 0; t < 5; t++) {
      if (scene->tracks[t][i].keys != NULL) {
        MemLock();
        MemFree(scene->tracks[t][i].keys, NULL, 0);
        MemUnlock();
        scene->tracks[t][i].keys = NULL;
      }
    }
  }
  BtlDemoSceneDeleteObjects(&scene->objects);
  BtlDemoSceneEventListClear(&scene->events);
}
