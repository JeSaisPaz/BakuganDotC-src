// bdc 0x088f4990 GameFieldCharSetAddActor
#include "bdc.h"

/* Spawns character-set entry `entry` in the next slot (`GameFieldSpawnPlacedActor` with the entry
   at `+0x80 + entry*0x2c + 4`), freezes it (virtual slot 16), increments the count and rebuilds the
   guard list. */

typedef struct CharSetEntry {
  u8 head[4];
  u8 body[0x28];
} CharSetEntry;

void GameFieldCharSetAddActor(void *mgr, u8 entry)

{
  GameFieldCharSet *set = (GameFieldCharSet *)mgr;
  GameFieldPlacement *obj;
  const VtblEntry *ve;

  GameFieldCharSetHasEntry(mgr,entry);
  GameFieldSpawnPlacedActor(mgr,set->placedCount,entry,((CharSetEntry *)set->entries[0])[entry].body,0);
  obj = set->actors[set->placedCount];
  ve = &obj->vtbl[16];
  ((void (*)(void *))ve->fn)((u8 *)obj + ve->delta);
  set->placedCount++;
  GameFieldCharSetBuildGuardList(mgr);
  return;
}
