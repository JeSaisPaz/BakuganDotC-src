// bdc 0x088fbbd8 GameQuestFreeCamTable
#include "bdc.h"

/* Frees a quest camera table built by `GameQuestParseCamTable` (the `.cptb` half of the
   quest-field camera system loaded by `GameFieldCameraLoadQuestCam`): for every camera set of
   the table (`GameQuestCamPtrVec`) it frees each entry's embedded vector buffer `subData` and the
   entry, clears the slot, then frees the set's buffer and the set; finally frees the table's own
   buffer `data` and, when bit 0 of `flags` is set, the table itself (deleting-destructor
   convention). All frees are `MemFree` under `MemLock`. */

void GameQuestFreeCamTable(GameQuestCamTable *table, u32 flags)
{
  GameQuestCamPtrVec *set;
  GameQuestCamEntry *entry;
  GameQuestCamEntry **slot;
  void *buf;
  bool inRange;
  s32 i;
  s32 j;

  if (table == NULL) {
    return;
  }
  for (i = 0; i < table->count; i++) {
    /* inlined bounds-checked operator[] (lower bound only) */
    if (i > -1) {
      set = table->data[i];
    } else {
      memset(&g_gameQuestCamNullSet, 0, 4);
      set = g_gameQuestCamNullSet;
    }
    if (set == NULL) {
      continue;
    }
    for (j = 0; j < set->count; j++) {
      inRange = j > -1;
      if (inRange) {
        entry = set->data[j];
      } else {
        memset(&g_gameQuestCamNullEntry, 0, 4);
        entry = g_gameQuestCamNullEntry;
      }
      if (entry == NULL) {
        continue;
      }
      if (inRange && j < set->count) {
        entry = set->data[j];
      } else {
        memset(&g_gameQuestCamNullEntry, 0, 4);
        entry = g_gameQuestCamNullEntry;
      }
      if (entry != NULL) {
        /* the asm also tests &entry->subData != NULL (inlined member destructor) */
        buf = entry->subData;
        if (buf != NULL) {
          MemLock();
          MemFree(buf, NULL, 0);
          MemUnlock();
          entry->subData = NULL;
        }
        MemLock();
        MemFree(entry, NULL, 0);
        MemUnlock();
      }
      if (inRange && j < set->count) {
        slot = &set->data[j];
      } else {
        memset(&g_gameQuestCamNullEntry, 0, 4);
        slot = &g_gameQuestCamNullEntry;
      }
      *slot = NULL;
    }
    if (set != NULL) {
      buf = set->data;
      if (buf != NULL) {
        MemLock();
        MemFree(buf, NULL, 0);
        MemUnlock();
        set->data = NULL;
      }
      MemLock();
      MemFree(set, NULL, 0);
      MemUnlock();
    }
  }
  if (table != NULL) {
    buf = table->data;
    if (buf != NULL) {
      MemLock();
      MemFree(buf, NULL, 0);
      MemUnlock();
      table->data = NULL;
    }
  }
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(table, NULL, 0);
    MemUnlock();
  }
}
