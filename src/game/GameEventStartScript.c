// bdc 0x088ef5c0 GameEventStartScript
#include "bdc.h"

/* Starts a command list when the event is idle (index `+0x250` at the end `+0x252`): sets the
   command pointer `+0x20` and count, resets the message queue, the HUD flag, the blocking flag and
   coordinate mode, initialises the fade state and enters state 2. Returns 1 when started. */

s32 GameEventStartScript(GameEvent *self, void *cmds, u16 count)

{
  if (self->cmdEnd <= self->cmdIndex) {
    self->cmdIndex = 0;
    self->cmdEnd = count;
    self->cmd = cmds;
    self->msgState = '\0';
    self->msgPos = 0;
    self->msgCount = 0;
    UiTalkBalloonSetPortraitEnabled('\0');
    self->blocking = '\0';
    self->coordMode = '\0';
    GameEventInitFadeState(self);
    self->state = '\x02';
    return 1;
  }
  return 0;
}

