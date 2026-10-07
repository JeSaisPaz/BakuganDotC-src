// bdc 0x088e1d04 ActorPlayerDropBall
#include "bdc.h"

/* Cancels a throw in progress: destroys the object `+0x4d0`, clears `+0x4a8`, releases the ball
   `+0x418` (`ActorBallCreate`) (`ActorBallSetIdle`), resets the trail `+0x4a4` and clears `+0x4aa`.
   Called by the field input handler. */

void ActorPlayerDropBall(ActorPlayer *self)

{
  CoreObject *aim;
  CoreObject *ball;
  
  aim = (CoreObject *)self->aimSprite;
  if (aim != (CoreObject *)0x0) {
    /* deleting destructor (vtable entry 1), flag 3 */
    const VtblEntry *dtor = &((const VtblEntry *)aim->vtable)[1];
    ((void (*)(void *, int))dtor->fn)((char *)aim + dtor->delta, 3);
    self->aimSprite = (void *)0x0;
  }
  ball = (CoreObject *)self->ball;
  self->throwing = '\0';
  if (ball != (CoreObject *)0x0) {
    ActorBallSetIdle(ball);
  }
  if (self->trail != (void *)0x0) {
    ActorBallTrailReset(self->trail,false);
  }
  self->aimState[0] = '\0';
  return;
}
