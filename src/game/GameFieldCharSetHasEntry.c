// bdc 0x088f4944 GameFieldCharSetHasEntry
#include "bdc.h"

/* True when a placed character comes from character-set entry `entry` (`placement+0x3c`). */

s32 GameFieldCharSetHasEntry(void *mgr, u8 entry)
{
  GameFieldPlacedChar **table = (GameFieldPlacedChar **)g_gameEventLocationBlock;
  u8 i = 0;

  do {
    if (table[i]->entry == entry) {
      return 1;
    }
    i++;
  } while (i < ((GameFieldCharSet *)mgr)->placedCount);
  return 0;
}
