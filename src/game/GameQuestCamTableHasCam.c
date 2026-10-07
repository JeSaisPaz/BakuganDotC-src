// bdc 0x088fbef8 GameQuestCamTableHasCam
#include "bdc.h"

/* Returns 1 when the current camera set of the quest camera table (`table+0xc`, pointer vector
   `{data, cap, count}`) holds an entry whose id `+0x3c` equals `camId`, else 0. */

int GameQuestCamTableHasCam(void *table, int camId)

{
  GameQuestCamPtrVec *vec;
  GameQuestCamEntry *entry;
  int i;

  vec = ((GameQuestCamTable *)table)->cur;
  i = 0;
  if (0 < vec->count) {
    do {
      if ((i < 0) || (vec->count <= i)) {
        memset(&g_gameQuestCamNullEntry, 0, 4);
        entry = g_gameQuestCamNullEntry;
      }
      else {
        entry = vec->data[i];
      }
      if (entry->id == camId) {
        return 1;
      }
      vec = ((GameQuestCamTable *)table)->cur;
      i = i + 1;
    } while (i < vec->count);
  }
  return 0;
}
