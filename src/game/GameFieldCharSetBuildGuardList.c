// bdc 0x088f42e0 GameFieldCharSetBuildGuardList
#include "bdc.h"

/* Rebuilds the list `+0xa9` (count `guardCount`) of character slots whose placement marks them as guards
   (`isGuard`). */

void GameFieldCharSetBuildGuardList(void *mgr)
{
  GameFieldCharSet *set = (GameFieldCharSet *)mgr;
  GameFieldPlacedChar **table = (GameFieldPlacedChar **)g_gameEventLocationBlock;
  u8 count = set->placedCount;
  u8 i = 0;

  set->guardCount = 0;
  do {
    u8 n = set->guardCount;
    if (table[i]->isGuard != 0) {
      set->events[0x20 + n] = i;
      set->guardCount = n + 1;
    }
    i++;
  } while (i < count);
}
