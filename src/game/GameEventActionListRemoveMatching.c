// bdc 0x088eaf68 GameEventActionListRemoveMatching
#include "bdc.h"

/* Removes (newest first, `GameEventActionListRemoveAt`) every action whose kind (slot 5) is
   `kind` and whose target index (slot 6) is `target`. */

void GameEventActionListRemoveMatching(GameEventActionList *self, s32 kind, u8 target)
{
  u32 i = self->count;

  while (i != 0) {
    GameEventAction *action;
    const VtblEntry *e;

    i = (i - 1) & 0xff;
    action = self->actions[i];
    e = &action->vtbl[5];
    if (((s32 (*)(void *))e->fn)((u8 *)action + e->delta) == kind) {
      action = self->actions[i];
      e = &action->vtbl[6];
      if (((u32 (*)(void *))e->fn)((u8 *)action + e->delta) == target) {
        GameEventActionListRemoveAt(self, (u8)i);
      }
    }
  }
}
