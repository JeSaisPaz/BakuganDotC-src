// bdc 0x088eea68 GameEventExecUntilWait
#include "bdc.h"

/* Event state 2 handler (from `GameEventUpdate`): clears the wait type `+0x269` and executes
   commands (`GameEventExecCommand`) until one starts a wait (returns 3) or the script ends
   (returns 5 when every queued message was shown, else 2). */

s32 GameEventExecUntilWait(GameEvent *self)

{
  bool more;
  GameEventCommand *cmd;
  s32 result;
  
  result = 2;
  self->waitType = '\0';
  more = self->cmdIndex < self->cmdEnd;
  while( true ) {
    if (!more) {
      if (self->msgPos == self->msgCount) {
        result = 5;
      }
      return result;
    }
    cmd = self->cmd;
    GameEventExecCommand(self,cmd->op,cmd->flag,cmd->arg);
    if (self->waitType != '\0') break;
    self->cmdIndex = self->cmdIndex + 1;
    self->cmd = self->cmd + 1;
    more = self->cmdIndex < self->cmdEnd;
  }
  return 3;
}

