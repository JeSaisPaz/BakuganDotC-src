// bdc 0x088e7df0 ActorNpcNextPatrolCommand
#include "bdc.h"

/* Patrol script step (vtable slot 35, `+0x11c`) of the field NPC/guard actor classes (models
   0x4e..0x53 `npc_sm_we/mi/st`, `npc_sr_we/mi/st`; base constructor `ActorNpcCtor`, vtables
   `0x08af3b74`, `0x08af39e4`, `0x08af3d04`, `0x08af3e94`): returns at once when
   `ActorNpcCheckInterrupt` took over. Otherwise, for a placement with `routeMode` 1, no
   `patrolHold` and a route, reads the 8-byte command at `routeStep` of the route (count word, then
   steps of four `s16`; the step wraps to 0 and clears `forceUpdate` at the end) and switches state:
   command 0 waits `n/2` frames (state 1); 1 walks to (x*0.2, z*0.2) at speed v*0.1/30 (state 2);
   2 turns (state 3): `lookHeading` = (a+90) degrees wrapped to (-pi, pi], `turnRate` = the wrapped
   angle pi/2 - b*2pi/65535 over 30 frames (3 on stage 0x11 for route indices 0x9d/0x9f, 90 on
   stage 9 for 0xa0/0x9f); 3 does nothing; 4 restarts state 6 only when already in it; other
   commands do nothing. Every path but the interrupt one ends with `ActorPlayMotion` slot 0,
   looped, blend 0.2. */

void ActorNpcNextPatrolCommand(ActorNpc *self)
{
  ActorNpcPlacement *placement;
  s32 *route;
  const s16 *step;
  u32 index;
  s16 cmd;
  s32 x;
  s32 z;
  s32 stage;
  float heading;
  float turn;
  float frames;

  if (ActorNpcCheckInterrupt(self, 0) != 0) {
    return;
  }
  placement = (ActorNpcPlacement *)self->base.placement;
  if (placement->routeMode == 0 || placement->routeMode >= 2 || self->base.patrolHold != 0) {
    goto play;
  }
  route = self->base.route;
  if (route == NULL) {
    goto play;
  }
  index = (u32)self->base.routeStep;
  if (index >= (u32)route[0]) {
    self->base.routeStep = 0;
    self->forceUpdate = 0;
    index = 0;
  }
  step = (const s16 *)(route + 1) + index * 4;
  cmd = step[0];
  if ((u32)(s32)cmd > 4) {
    goto play;
  }
  switch (cmd) {
  case 1:
    self->aiState = 2;
    self->subStep = 0;
    x = step[1];
    z = step[2];
    self->base.routeTarget[1] = 0.0f;
    self->base.routeTarget[3] = 0.0f;
    self->base.routeTarget[0] = (float)x * 0.2f;
    self->base.routeTarget[2] = (float)z * 0.2f;
    self->speed = (float)step[3] * 0.1f * 0.033333335f;
    break;
  case 2:
    self->aiState = 3;
    self->subStep = 0;
    heading = (float)(step[1] + 90) * 0.017453292f;
    self->lookHeading = heading;
    if (!(heading <= 3.1415927f)) {
      heading = heading - 6.2831855f;
    } else if (heading <= -3.1415927f) {
      heading = heading + 6.2831855f;
    }
    self->lookHeading = heading;
    frames = 30.0f;
    if (placement != NULL) {
      stage = g_scriptGlobalVars[1];
      if (stage == 0x11 && (placement->routeIndex == 0x9d || placement->routeIndex == 0x9f)) {
        frames = 3.0f;
      } else if (stage == 9 && (placement->routeIndex == 0xa0 || placement->routeIndex == 0x9f)) {
        frames = 90.0f;
      }
    }
    turn = -((float)step[2] * 6.2831855f * 1.5259022e-05f - 1.5707964f);
    if (!(turn <= 3.1415927f)) {
      turn = turn - 6.2831855f;
    } else if (turn <= -3.1415927f) {
      turn = turn + 6.2831855f;
    }
    self->turnRate = turn / frames;
    break;
  case 3:
    break;
  case 4:
    if (self->aiState == 6) {
      self->aiState = 6;
      self->subStep = 0;
    }
    break;
  default:
    self->aiState = 1;
    self->subStep = 0;
    self->timer = step[1] / 2;
    break;
  }
play:
  ActorPlayMotion(0.2f, self, 0, 1, 0);
}
