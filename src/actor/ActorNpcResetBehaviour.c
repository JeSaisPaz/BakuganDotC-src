// bdc 0x088e5cec ActorNpcResetBehaviour
#include "bdc.h"

/* Vtable slot 13 of the field NPC/guard classes (base `ActorNpcCtor`): clears the caught flags
   (`+0x354`/`+0x355`), phase, route, step, AI state and sub-step, removes the head effects,
   remembers the current heading as home heading `+0x414` and reloads the route
   (`ActorLookupRoute`). */

void ActorNpcResetBehaviour(ActorNpc *self)

{
  (self->base).alerted = '\0';
  (self->base).detected = '\0';
  self->phase = 0;
  (self->base).route = (s32 *)0x0;
  (self->base).routeStep = 0;
  self->aiState = 0;
  self->subStep = 0;
  ActorNpcShowHeadEffect(self,-1,'\x01','\x01');
  self->homeHeading = (self->base).base.rot[1];
  ActorLookupRoute(&self->base);
  return;
}

