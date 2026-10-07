// bdc 0x088f4888 GameFieldCharSetRestartAll
#include "bdc.h"

/* `ActorRestartPlacedBehaviour` on every placed actor. */

void GameFieldCharSetRestartAll(void *mgr)

{
  GameFieldCharSet *set = (GameFieldCharSet *)mgr;
  u32 i;

  i = 0;
  do {
    ActorRestartPlacedBehaviour((Actor *)set->actors[i]);
    i = (i + 1) & 0xff;
  } while ((s32)i < (s32)set->placedCount);
  return;
}
