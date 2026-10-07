// bdc 0x088dfa08 ActorGetList
#include "bdc.h"

/* Returns `g_actorList` (singleton accessor, named by `bdc singleton`). */

void *ActorGetList(void)

{
  return g_actorList;
}

