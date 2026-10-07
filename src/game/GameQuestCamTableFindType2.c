// bdc 0x088fc0f4 GameQuestCamTableFindType2
#include "bdc.h"

/* Returns the first entry of camera set `set` with id `+0x3c == camId` and type `+0x58 == 2`, or
   NULL. Twin of `GameQuestCamTableFindType1`. */

void *GameQuestCamTableFindType2(void *table, int camId, void *set)

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
        if (entry->type == 2) {
          return entry;
        }
      }
      i = i + 1;
    } while (i < vec->count);
  }
  return NULL;
}
