// bdc 0x088ead40 GameEventActionListUpdate
#include "bdc.h"

/* Per-frame update (slot 2) of the event action list task (`GameEventActionListCtor`): in state 1
   runs every action's update (virtual slot 4, `+0x24`) and removes the finished ones
   (`GameEventActionListRemoveAt`); in state 2 removes the task. */

void GameEventActionListUpdate(GameEventActionList *self)
{
  u8 state = self->state;

  if (state != 0) {
    if (state < 2) {
      s32 i = 0;

      while (i < self->count) {
        GameEventAction *action = self->actions[i];
        const VtblEntry *e = &action->vtbl[4];

        if (((s32 (*)(void *))e->fn)((u8 *)action + e->delta) != 0) {
          GameEventActionListRemoveAt(self, (u8)i);
        } else {
          i = (i + 1) & 0xff;
        }
      }
    } else if (state < 3) {
      CoreTaskRemove(&self->base, true);
    }
  }
}
