// bdc 0x088f4a30 GameFieldCharSetRemoveActor
#include "bdc.h"

/* Removes the actor spawned from entry `entry`: finds the placed slot whose record has that `entry`
   (does nothing if none), deletes its placement, unlinks and releases the actor, compacts the actor
   and placement tables (renumbering `slot` / `placementSlot`), clears the freed last slot and
   rebuilds the guard list. */

void GameFieldCharSetRemoveActor(void *mgr, u8 entry)
{
  GameFieldCharSet *set = (GameFieldCharSet *)mgr;
  GameFieldPlacedChar **table = (GameFieldPlacedChar **)g_gameEventLocationBlock;
  u8 count = set->placedCount;
  u8 i = 0;
  GameFieldPlacement **slot;

  do {
    if (table[i]->entry == entry) break;
    i++;
  } while (i < count);
  if (i >= count) return;

  GameFieldCharSetDeletePlacement(mgr, i);
  slot = &set->actors[i];
  CoreObjectUnlink((CoreObject *)*slot);
  CoreObjectDeferDelete((CoreObject *)*slot, 0);
  set->placedCount--;
  count = set->placedCount;
  while (i < count) {
    GameFieldPlacedChar *next = table[i + 1];
    GameFieldPlacement *actor;

    table[i] = next;
    next->slot = i;
    actor = set->actors[i + 1];
    set->actors[i] = actor;
    ((Actor *)actor)->placementSlot = i;
    i++;
    count = set->placedCount;
  }
  table[count] = NULL;
  set->actors[set->placedCount] = NULL;
  GameFieldCharSetBuildGuardList(mgr);
}
