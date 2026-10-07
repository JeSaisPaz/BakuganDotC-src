// bdc 0x088df9fc ActorClearList
#include "bdc.h"

/* Clears `g_actorList` (stores 0) (singleton accessor, named by `bdc singleton`). */

void ActorClearList(void)

{
  g_actorList = (void *)0x0;
  return;
}

