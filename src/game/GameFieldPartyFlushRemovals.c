// bdc 0x088c23ec GameFieldPartyFlushRemovals
#include "bdc.h"

/* Pops every queued removal from the party manager `g_gameFieldCharSet` (`GameFieldCharSetPopEvent`, -1 when empty)
   and removes each member (`GameFieldPartyRemove`). */

void GameFieldPartyFlushRemovals(CoreTask *task)
{
  s32 id;

  while ((id = GameFieldCharSetPopEvent(g_gameFieldCharSet)) != -1) {
    GameFieldPartyRemove(task, (u8)id);
  }
}
