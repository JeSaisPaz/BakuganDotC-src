// bdc 0x088f3fb0 GameFieldCharSetCtor
#include "bdc.h"

/* Constructor of the field character-set manager: clears the actor table, the entry
   buffer, the pause flag, the event stack, the guard list and the counts, and registers itself in
   `g_gameFieldCharSet`. Called by `GameFieldPhaseLoad`. */

void *GameFieldCharSetCtor(void *mgr)

{
  GameFieldCharSet *set = (GameFieldCharSet *)mgr;

  set->entries[0] = 0;
  set->entries[1] = 0;
  set->paused = 0;
  set->placedCount = 0;
  set->guardCount = 0;
  set->eventDepth = 0;
  g_gameFieldCharSet = set;
  memset(set, 0, 0x80);
  memset(&set->events[0x20], 0, 0x20);
  memset(&set->events[0], 0, 0x20);
  return mgr;
}
