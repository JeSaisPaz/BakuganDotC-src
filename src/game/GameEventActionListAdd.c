// bdc 0x088eaf14 GameEventActionListAdd
#include "bdc.h"

/* Appends `action` to the list and starts it (its virtual slot 2). */

void GameEventActionListAdd(GameEventActionList *self, GameEventAction *action)

{
  const VtblEntry *e;

  self->actions[self->count] = action;
  e = &action->vtbl[2];
  ((void (*)(void *))e->fn)((u8 *)action + e->delta);
  self->count = self->count + 1;
}
