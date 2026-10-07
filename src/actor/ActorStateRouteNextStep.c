// bdc 0x088e0ba0 ActorStateRouteNextStep
#include "bdc.h"

/* Route step state handler (vtable slot 30, 7 vtables): for actors whose placement record has
   `routeMode` 1 and a loaded route (`+0x35c`: count word, then 8-byte steps of four `s16`), reads
   the step at `routeStep`. Past the last step it wraps to 0 (4 for model 0x4d), unless `routeEnds`
   is set: then it clears `routeMode`, `routeGroup`, the route and `routeEnds`, goes idle (state 0)
   and zeroes the velocity (bank constant C720, stops the actor), and returns. Step command 0:
   idle (state 0) and wait `step[1]/2` frames; 1: state 10, walk to (step[1]*0.2, step[2]*0.2) at
   speed `step[3]*0.1/30`; other commands do nothing. */

void ActorStateRouteNextStep(Actor *self)
{
  ActorNpcPlacement *placement;
  s32 *route;
  const s16 *step;
  s16 cmd;
  s32 x;
  s32 z;

  placement = (ActorNpcPlacement *)self->placement;
  if (placement->routeMode != 1) {
    return;
  }
  route = self->route;
  if (route == NULL) {
    return;
  }
  if ((u32)self->routeStep >= (u32)route[0]) {
    if (self->routeEnds != 0) {
      placement->routeMode = 0;
      ((ActorNpcPlacement *)self->placement)->routeGroup = 0;
      self->route = NULL;
      self->routeEnds = 0;
      ActorSetStateBase(self, 0, 0);
      self->base.velocity[0] = 0.0f;
      self->base.velocity[1] = 0.0f;
      self->base.velocity[2] = 0.0f;
      self->base.velocity[3] = 0.0f;
      return;
    }
    self->routeStep = 0;
    if (self->base.base.unk08 == 0x4d) {
      self->routeStep = 4;
    }
  }
  cmd = ((const s16 *)(route + 1))[self->routeStep * 4];
  if (cmd > 0) {
    if (cmd < 2) {
      ActorSetStateBase(self, 10, 0);
      step = (const s16 *)(self->route + 1) + self->routeStep * 4;
      self->waitTimer = 0;
      x = step[1];
      z = step[2];
      self->routeTarget[1] = 0.0f;
      self->routeTarget[3] = 0.0f;
      self->routeTarget[0] = (float)x * 0.2f;
      self->routeTarget[2] = (float)z * 0.2f;
      self->walkSpeed = (float)step[3] * 0.1f * 0.033333335f;
    }
  } else if (cmd == 0) {
    ActorSetStateBase(self, 0, 0);
    step = (const s16 *)(self->route + 1) + self->routeStep * 4;
    self->waitTimer = step[1] / 2;
  }
}
