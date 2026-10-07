// bdc 0x088e1a9c ActorPlayerFindTalkTarget
#include "bdc.h"

/* Finds the actor the player can talk to: from a point 7 units in front of the player (heading
   `rot[1]`: x += 7 cos, z += 7 sin), picks the nearest other actor of the actor list
   (`ActorGetList`) by horizontal (XZ) squared distance that lies within 2x/4x/3x the player's
   `separationRadius` (sector 1 / 0 / other of `ActorGetFacingSectorFrom`); guards (models
   0x4e..0x53) and actors whose placement `charCode` is 7 or 10 are rejected. Sets
   `talkTargetInReach` and returns the actor, or 0 (also at once while stealthed,
   `ActorPlayerIsStealthed`, or when the player is not in state 0, 1 or 6). */

void *ActorPlayerFindTalkTarget(ActorPlayer *self)
{
  float point[3];
  float heading;
  float dx;
  float dz;
  float dist;
  Actor *actor;
  Actor *best;
  float bestDist;
  float radius;

  if (ActorPlayerIsStealthed(self) != 0) {
    return NULL;
  }
  if (self->base.state != 0 && self->base.state != 1 && self->base.state != 6) {
    return NULL;
  }
  actor = *(Actor **)ActorGetList();
  best = NULL;
  bestDist = __builtin_inff();
  heading = self->base.base.rot[1];
  point[0] = __builtin_cosf(heading) * 7.0f + self->base.base.pos[0];
  point[1] = 0.0f * 7.0f + self->base.base.pos[1];
  point[2] = __builtin_sinf(heading) * 7.0f + self->base.base.pos[2];

  for (; actor != NULL; actor = (Actor *)actor->base.base.next) {
    if (actor == &self->base) {
      continue;
    }
    /* y difference cleared: XZ squared distance */
    dx = point[0] - actor->base.pos[0];
    dz = point[2] - actor->base.pos[2];
    dist = dx * dx + 0.0f + dz * dz;
    radius = self->base.separationRadius;
    if (ActorGetFacingSectorFrom(&self->base, actor->base.pos) == 1) {
      radius = radius * 2.0f;
    } else if (ActorGetFacingSectorFrom(&self->base, actor->base.pos) != 0) {
      radius = radius * 3.0f;
    } else {
      radius = radius * 4.0f;
    }
    if (dist < radius * radius && dist < bestDist) {
      bestDist = dist;
      best = actor;
    }
  }

  if (best == NULL) {
    return NULL;
  }
  if (best->base.base.unk08 >= 0x4e && best->base.base.unk08 < 0x54) {
    best = NULL;
  } else if (((ActorNpcPlacement *)best->placement)->charCode == 7) {
    best = NULL;
  } else if (((ActorNpcPlacement *)best->placement)->charCode == 10) {
    best = NULL;
  }
  if (best != NULL) {
    self->talkTargetInReach = 1;
  }
  return best;
}
