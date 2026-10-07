// bdc 0x088eac54 GameEventActionListRemoveAt
#include "bdc.h"

/* Stops (slot 3), deletes (slot 1, flags 3) and removes the action at `index`, shifting the later
   ones down and decrementing the count `+0x51`. */

void GameEventActionListRemoveAt(GameEventActionList *self, u8 index)

{
  u32 i = index;
  u32 count;
  GameEventAction *action;

  if (i < self->count && self->actions[i] != NULL) {
    const VtblEntry *stop;

    action = self->actions[i];
    stop = &action->vtbl[3];
    ((void (*)(void *, int))stop->fn)((u8 *)action + stop->delta, 0);
    action = self->actions[i];
    if (action != NULL) {
      const VtblEntry *del = &action->vtbl[1];

      ((void (*)(void *, int))del->fn)((u8 *)action + del->delta, 3);
      self->actions[i] = NULL;
    }
    count = self->count;
    for (; i < count; i = (i + 1) & 0xff) {
      self->actions[i] = self->actions[i + 1];
    }
    self->actions[count] = NULL;
    self->count = count - 1;
  }
  return;
}
