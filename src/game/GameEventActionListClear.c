// bdc 0x088eae54 GameEventActionListClear
#include "bdc.h"

/* Stops and deletes every action of the 16 slots and resets count and state to 0. */

void GameEventActionListClear(GameEventActionList *self)

{
  u8 i;

  for (i = 0; i < 0x10; i++) {
    GameEventAction *action = self->actions[i];

    if (action != NULL) {
      const VtblEntry *stop = &action->vtbl[3];

      ((void (*)(void *, int))stop->fn)((u8 *)action + stop->delta, 0);
      action = self->actions[i];
      if (action != NULL) {
        const VtblEntry *del = &action->vtbl[1];

        ((void (*)(void *, int))del->fn)((u8 *)action + del->delta, 3);
        self->actions[i] = NULL;
      }
    }
  }
  self->count = 0;
  self->state = 0;
  return;
}
