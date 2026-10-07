// bdc 0x088eebe4 GameEventCheckWaitDone
#include "bdc.h"

/* Event state 3 step: unless skipping, tests the current wait (`+0x269`): 1 frame counter `+0x260`,
   2 all queued messages shown, 4 `+0x40` cleared, others ask virtual slot 12; when done returns to
   state 2 and steps past the waiting command. */

void GameEventCheckWaitDone(GameEvent *self)

{
  u8 type;
  s32 done;

  if ((self->flags & 1) != 0) {
    return;
  }
  type = self->waitType;
  done = 0;
  if (type < 3) {
    if (type != 0) {
      if (type < 2) {
        self->waitFrames = self->waitFrames - 1;
        if (self->waitFrames <= 0) {
          done = 1;
        }
      }
      else if (self->msgPos == self->msgCount) {
        self->msgAutoTime = 0;
        done = 1;
      }
      goto check;
    }
  } else if (type == 4) {
    if (self->msgRequest == 0) {
      done = 1;
    }
    goto check;
  }
  {
    const VtblEntry *slot = &((const VtblEntry *)self->base.vtable)[12];

    done = ((s32 (*)(void *))slot->fn)((u8 *)self + slot->delta);
  }
check:
  if (done != 0) {
    self->state = 2;
    self->cmdIndex = self->cmdIndex + 1;
    self->cmd = self->cmd + 1;
  }
  return;
}
