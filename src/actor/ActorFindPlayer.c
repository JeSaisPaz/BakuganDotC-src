// bdc 0x088e1908 ActorFindPlayer
#include "bdc.h"

/* Returns the player-controlled actor: the first actor in the global actor list (`ActorGetList`)
   whose byte `+0x14c` is set (actors spawned by `ActorSpawn` with `flag == 0`), or NULL. */

void *ActorFindPlayer(void)
{
  Actor **head;
  Actor *actor;

  if (ActorGetList() != NULL) {
    head = (Actor **)ActorGetList();
    actor = *head;
    while (actor != NULL) {
      if (actor->isPlayer != 0) {
        return actor;
      }
      actor = (Actor *)actor->base.base.next;
    }
  }
  return NULL;
}
