// bdc 0x088eb0b8 GameEventActionListStopAll
#include "bdc.h"

/* Stops and deletes every active action and resets the count. Called by `GameEventRunCommands`
   and `GameEventSlot7`. */

void GameEventActionListStopAll(GameEventActionList *self)

{
  u8 i = 0;

  if (self->count != 0) {
    do {
      GameEventAction *action = self->actions[i];
      const VtblEntry *stop = &action->vtbl[3];

      ((void (*)(void *, int))stop->fn)((u8 *)action + stop->delta, 0);
      action = self->actions[i];
      if (action != NULL) {
        const VtblEntry *del = &action->vtbl[1];

        ((void (*)(void *, int))del->fn)((u8 *)action + del->delta, 3);
        self->actions[i] = NULL;
      }
      i++;
    } while (i < self->count);
  }
  self->count = 0;
  return;
}
