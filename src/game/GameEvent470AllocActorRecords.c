// bdc 0x088efb54 GameEvent470AllocActorRecords
#include "bdc.h"

/* Allocates (once) the 0x1300-byte block of 0x4c-byte event-actor records `+0x284` and clears one
   record per placed character (count `mgr+0xc9`), mapping each character-set entry index
   (`placement+0x3c`) to its record in the table `+0x292`. */

void GameEvent470AllocActorRecords(GameEvent470 *self)

{
  GameFieldPlacedChar **table = (GameFieldPlacedChar **)g_gameEventLocationBlock;
  u8 i;

  if (self->actors == NULL) {
    bool fromLow;
    void *block;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    block = MemAlloc(0x1300, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->actors = block;
  }
  for (i = 0; i < g_gameFieldCharSet->placedCount; i++) {
    memset(&self->actors[i], 0, sizeof(GameEventActorRecord));
    self->actorMap[table[i]->entry] = i;
  }
  return;
}
