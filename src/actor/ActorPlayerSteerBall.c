// bdc 0x088e3d5c ActorPlayerSteerBall
#include "bdc.h"

/* Homing of the thrown ball: counts frames in `ballFrames` and, when `ballTarget` is set, walks the
   actor list; once it meets the target with its active check (vtable entry 11) true and its
   `ActorNpc` `aiState` not 8, it sets the ball's velocity to (aim - `ballStart`) / 18, where aim
   is the target position moved 6.3 units short of the target's matrix point +12 in y (direction
   normalised and clamped to [-1, 1] per lane, 0 when the two points coincide), keeping the ball's
   vertical speed; velocity w becomes the target position's w. Returns without steering otherwise. */

void ActorPlayerSteerBall(ActorPlayer *self)
{
  const VtblEntry *vtbl;
  ActorBall *ball;
  Actor *actor;
  float tgtPos[4];
  float head[4];
  float diff[3];
  float dir[3];
  float aim[3];
  float distSq;
  float lenSq;
  float scale;
  float dist;
  float rcp;
  float velY;

  self->ballFrames++;
  if (self->ballTarget == NULL) {
    return;
  }
  for (actor = *(Actor **)ActorGetList(); actor != NULL;
       actor = (Actor *)actor->base.base.next) {
    vtbl = &((const VtblEntry *)actor->base.base.vtable)[11];
    if (((s32 (*)(void *))vtbl->fn)((u8 *)actor + vtbl->delta) == 0) {
      continue;
    }
    if (self->ballTarget != actor) {
      continue;
    }
    if (((ActorNpc *)self->ballTarget)->aiState == 8) {
      continue;
    }

    /* tgtPos = target position; head = target matrix point, +12 in y */
    tgtPos[0] = actor->base.pos[0];
    tgtPos[1] = actor->base.pos[1];
    tgtPos[2] = actor->base.pos[2];
    tgtPos[3] = actor->base.pos[3];
    head[0] = actor->mtx[12];
    head[1] = actor->mtx[13];
    head[2] = actor->mtx[14];
    head[3] = actor->mtx[15];
    head[1] = head[1] + 12.0f;

    diff[0] = head[0] - tgtPos[0];
    diff[1] = head[1] - tgtPos[1];
    diff[2] = head[2] - tgtPos[2];
    distSq = diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2];

    /* dir = normalize(head - tgtPos), clamped per lane; scale 0 (bank S713) for a zero vector */
    lenSq = diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2];
    if (lenSq == 0.0f) {
      scale = 0.0f;
    } else {
      scale = VfRsq(lenSq);
    }
    dir[0] = VfSat1(diff[0] * scale);
    dir[1] = VfSat1(diff[1] * scale);
    dir[2] = VfSat1(diff[2] * scale);

    dist = __builtin_sqrtf(distSq) - 6.3f;
    aim[0] = tgtPos[0] + dir[0] * dist;
    aim[1] = tgtPos[1] + dir[1] * dist;
    aim[2] = tgtPos[2] + dir[2] * dist;

    rcp = VfRcp(18.0f);
    ball = (ActorBall *)self->ball;
    velY = ball->base.velocity[1];
    ball->base.velocity[0] = (aim[0] - self->ballStart[0]) * rcp;
    ball->base.velocity[1] = (aim[1] - self->ballStart[1]) * rcp;
    ball->base.velocity[2] = (aim[2] - self->ballStart[2]) * rcp;
    ball->base.velocity[3] = tgtPos[3];
    ((ActorBall *)self->ball)->base.velocity[1] = velY;
    return;
  }
}
