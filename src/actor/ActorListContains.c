// bdc 0x088dfcc8 ActorListContains
#include "bdc.h"

/* Returns `actor` if it is a member of the actor chain `*g_actorList` (walked through `+4 next`),
   else 0; the validity check for actor pointers that script opcodes read from variables.
   Counterpart of `BtlBakuganListFind` for the battle Bakugan chain. */

void *ActorListContains(void *actor)
{
  CoreObject *node;

  for (node = *(CoreObject **)g_actorList; node != 0; node = node->next) {
    if ((void *)node == actor) {
      return node;
    }
  }
  return 0;
}
