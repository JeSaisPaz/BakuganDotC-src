// bdc 0x088e1d84 ActorPlayerResetBehaviour
#include "bdc.h"

/* Vtable slot 13 of the player actor (`ActorPlayerCtor`): clears `+0x540` to 0xff and the
   caught/flag bytes `+0x354`, `+0x355`, `+0x3af`, returns to idle (`ActorSetState`), sets the
   motion helper's event flag (`+0xc = 1`), stops the stick-as-dpad mode, drops the ball
   (`ActorPlayerDropBall`) and clears the talk target `+0x414`. */

void ActorPlayerResetBehaviour(ActorPlayer *self)

{
  self->behaviourMark = 0xff;
  (self->base).detected = '\0';
  (self->base).alerted = '\0';
  self->throwLocked = '\0';
  ActorSetState(&self->base,0,'\0');
  ((BtlInput *)(self->base).input)->disabled = 1;
  g_padState->stickEmulatesDpad = '\0';
  ActorPlayerDropBall(self);
  self->talkTarget = (void *)0x0;
  return;
}
