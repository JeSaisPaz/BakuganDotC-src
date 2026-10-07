// bdc 0x088fbff4 GameQuestCamTableFindType1
#include "bdc.h"

/* Returns the first entry of camera set `set` (pointer vector `{data, cap, count}`) with id `+0x3c
   == camId` and type `+0x58 == 1`, or NULL. `table` is unused. */

void *GameQuestCamTableFindType1(void *table, int camId, void *set)

{
  GameQuestCamPtrVec *vec = (GameQuestCamPtrVec *)set;
  GameQuestCamEntry *entry;
  int i;

  i = 0;
  if (0 < vec->count) {
    do {
      if (i < 0) {
        memset(&g_gameQuestCamNullEntry, 0, 4);
        entry = g_gameQuestCamNullEntry;
      }
      else {
        entry = vec->data[i];
      }
      if (entry->id == camId) {
        if (entry->type == 1) {
          return entry;
        }
      }
      i = i + 1;
    } while (i < vec->count);
  }
  return NULL;
}
