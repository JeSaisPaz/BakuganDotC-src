// bdc 0x088f4c14 GameFieldCharSetSetActorVisible
#include "bdc.h"

/* Sets the visible byte `+0xb8` of the actor spawned from entry `entry`. */

void GameFieldCharSetSetActorVisible(void *mgr, u8 entry, u8 visible)
{
  GameFieldPlacedChar **table = (GameFieldPlacedChar **)g_gameEventLocationBlock;
  Actor **actors = (Actor **)mgr;
  u8 count = ((GameFieldCharSet *)mgr)->placedCount;
  u8 i = 0;

  do {
    if (table[i]->entry == entry) {
      break;
    }
    i = i + 1;
  } while (i < count);
  if (i < count) {
    actors[i]->base.visible = visible;
  }
}
