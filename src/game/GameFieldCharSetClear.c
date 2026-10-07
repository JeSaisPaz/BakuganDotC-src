// bdc 0x088f46cc GameFieldCharSetClear
#include "bdc.h"

/* Deletes every placement record, resets the counts and frees the entry buffer `+0x84`. */

void GameFieldCharSetClear(void *mgr)

{
  GameFieldCharSet *set = (GameFieldCharSet *)mgr;
  void *ptr;
  u8 slot;

  set->guardCount = 0;
  if (set->placedCount != 0) {
    slot = 0;
    do {
      GameFieldCharSetDeletePlacement(mgr, slot);
      slot = slot + 1;
    } while (slot < set->placedCount);
    set->placedCount = 0;
  }
  ptr = set->entries[1];
  if (ptr != NULL) {
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    set->entries[1] = NULL;
  }
  set->entries[0] = NULL;
}
