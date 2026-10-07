// bdc 0x088d8c58 GameFieldPointUpdate
#include "bdc.h"

/* Spawns the marker effect of a field point (`GameFieldPointSpawnEffect`) into `+0x18` when it is
   enabled and has none yet. Caller: `GameFieldPointListUpdate`. */

void GameFieldPointUpdate(void *point)

{
  GameFieldPoint *p = (GameFieldPoint *)point;

  if ((p->disabled == 0) && (p->effect == (void *)0x0)) {
    p->effect = GameFieldPointSpawnEffect(point);
  }
  return;
}
