// bdc 0x088eb02c GameEventActionListRemoveKind
#include "bdc.h"

/* Removes every action whose kind (slot 5) is `kind`. */

void GameEventActionListRemoveKind(GameEventActionList *self, s32 kind)
{
  u32 i = self->count;

  while (i != 0) {
    GameEventAction *action;
    const VtblEntry *e;

    i = (i - 1) & 0xff;
    action = self->actions[i];
    e = &action->vtbl[5];
    if (((s32 (*)(void *))e->fn)((u8 *)action + e->delta) == kind) {
      GameEventActionListRemoveAt(self, (u8)i);
    }
  }
}
