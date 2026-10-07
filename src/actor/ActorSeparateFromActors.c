// bdc 0x088dcb44 ActorSeparateFromActors
#include "bdc.h"

/* Keeps an actor from overlapping the others. Does nothing when the actor has no `bodyCollider` or
   its flag 0x40 (no push-apart) is set. Otherwise walks `g_actorList` and, for every other actor
   that is not model id 0x54 and has a `bodyCollider` without flag 4, whose vertical offset is under
   28.8 units: the horizontal offset self − other is taken, and if it is shorter than
   2·`separationRadius` (and longer than 0.001) a push along it of
   min(2r − dist, 100)·0.7·weight is added to an accumulator (weight = vertical overlap / 24, at
   most 1); coincident actors (dist ≤ 0.001) are pushed 5·weight along the reversed heading
   `rot[1]` + π, or along the heading itself when `CollisionRaycastRayB` from the body sphere
   centre along the reversed heading (length 2r) hits. When self `isPlayer`, every actor that
   caused a push gets `pushed` = 1 (self's own `pushed` is cleared at entry). If any push happened,
   the accumulator is clamped to length `separationRadius`, applied with `ActorMoveWithCollision`
   (with `flags` 0x10 set and the collider's 0x40 set for the call, restored if it was clear), and
   when the push opposes the horizontal part of `delta`, `delta` is scaled by
   1 / (1 − 6·|push|·cos), cos being the cosine between the two directions (delta[3] is written 0).
   VFPU bank constants read: C720 (zero accumulator), S703 (2/π, angle scale for vrot), S713 (0). */

void ActorSeparateFromActors(Actor *self, float *delta)
{
  CollisionCollider *collider;
  CollisionSphere *sphere;
  Actor *other;
  s32 anyPush;
  s32 pushedThis;
  s32 wasNoPush;
  float overlap;
  float weight;
  float push;
  float heading;
  float zero;
  float scale;
  float len;
  float inv;
  float dist;
  float lenSq;
  float cosv;
  void *hit;
  float accum[4];
  float tmp[4];
  float diff[4];
  float origin[4];
  float moveDelta[4];
  float dirXZ[4];

  self->pushed = 0;
  collider = (CollisionCollider *)self->bodyCollider;
  if (collider == NULL) {
    return;
  }
  if ((((CollisionCollider *)self->bodyCollider)->flags & 0x40) != 0) {
    return;
  }
  other = *(Actor **)g_actorList;
  anyPush = 0;
  accum[0] = 0.0f;
  accum[1] = 0.0f;
  accum[2] = 0.0f;
  accum[3] = 0.0f;
  for (; other != NULL; other = (Actor *)other->base.base.next) {
    pushedThis = 0;
    if (other == self) {
      continue;
    }
    if (other->base.base.unk08 == 0x54) {
      continue;
    }
    if (other->bodyCollider == NULL) {
      continue;
    }
    if ((((CollisionCollider *)other->bodyCollider)->flags & 4) != 0) {
      continue;
    }
    /* diff = self->pos - other->pos (xyz, w from self), staged through tmp */
    tmp[0] = self->base.pos[0] - other->base.pos[0];
    tmp[1] = self->base.pos[1] - other->base.pos[1];
    tmp[2] = self->base.pos[2] - other->base.pos[2];
    tmp[3] = self->base.pos[3];
    diff[0] = tmp[0];
    diff[1] = tmp[1];
    diff[2] = tmp[2];
    diff[3] = tmp[3];
    overlap = __builtin_fabsf(diff[1]) - 28.800001f;
    if (!(overlap < 0.0f)) {
      continue;
    }
    weight = overlap * -0.041666668f;
    if (!(weight <= 1.0f)) {
      weight = 1.0f;
    }
    diff[1] = 0.0f;
    dist = __builtin_sqrtf(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
    if (dist < self->separationRadius * 2.0f && !(dist <= 0.001f)) {
      push = self->separationRadius * 2.0f - dist;
      if (!(push <= 100.0f)) {
        push = 100.0f;
      }
      weight = push * 0.7f * weight;
      /* diff = normalize(diff) * weight (w = S713 = 0); accum += diff (xyz) */
      lenSq = diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2];
      inv = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
      scale = inv * weight;
      diff[0] = diff[0] * scale;
      diff[1] = diff[1] * scale;
      diff[2] = diff[2] * scale;
      diff[3] = 0.0f;
      accum[0] = accum[0] + diff[0];
      accum[1] = accum[1] + diff[1];
      accum[2] = accum[2] + diff[2];
      anyPush = 1;
      pushedThis = 1;
    } else if (dist <= 0.001f) {
      /* origin = body sphere centre (all four words) */
      sphere = (CollisionSphere *)((CollisionCollider *)self->bodyCollider)->shapeDesc;
      origin[0] = sphere->center[0];
      origin[1] = sphere->center[1];
      origin[2] = sphere->center[2];
      origin[3] = sphere->radiusSq;
      heading = self->base.rot[1] + 3.14159274f;
      if (!(heading <= 3.14159274f)) {
        heading = heading - 6.28318548f;
      } else if (heading <= -3.14159274f) {
        heading = heading + 6.28318548f;
      }
      /* diff = (cos, 0, sin)(heading) * 2r (vrot of heading·2/π) */
      len = self->separationRadius * 2.0f;
      diff[0] = __builtin_cosf(heading) * len;
      diff[1] = 0.0f * len;
      diff[2] = __builtin_sinf(heading) * len;
      diff[3] = 0.0f;
      hit = CollisionRaycastRayB(self->collisionMask, origin, diff);
      if (hit != NULL) {
        heading = self->base.rot[1];
      }
      /* diff = (cos, 0, sin)(heading) * 5·weight */
      len = weight * 5.0f;
      diff[0] = __builtin_cosf(heading) * len;
      diff[1] = 0.0f * len;
      diff[2] = __builtin_sinf(heading) * len;
      diff[3] = 0.0f;
      diff[1] = 0.0f;
      accum[0] = accum[0] + diff[0];
      accum[1] = accum[1] + diff[1];
      accum[2] = accum[2] + diff[2];
      anyPush = 1;
      pushedThis = 1;
    }
    if (pushedThis != 0 && self->isPlayer != 0) {
      other->pushed = 1;
    }
  }
  if (anyPush == 0) {
    return;
  }
  lenSq = accum[0] * accum[0] + accum[1] * accum[1] + accum[2] * accum[2];
  zero = 0.0f;
  if (!(lenSq <= self->separationRadius * self->separationRadius)) {
    /* accum = normalize(accum) * separationRadius (w = S713 = 0) */
    len = accum[0] * accum[0] + accum[1] * accum[1] + accum[2] * accum[2];
    inv = (len == 0.0f) ? 0.0f : VfRsq(len);
    scale = inv * self->separationRadius;
    accum[0] = accum[0] * scale;
    accum[1] = accum[1] * scale;
    accum[2] = accum[2] * scale;
    accum[3] = 0.0f;
  }
  collider = (CollisionCollider *)self->bodyCollider;
  wasNoPush = (collider->flags & 0x40) != 0;
  self->flags |= 0x10;
  collider->flags |= 0x40;
  moveDelta[0] = accum[0];
  moveDelta[1] = accum[1];
  moveDelta[2] = accum[2];
  moveDelta[3] = accum[3];
  ActorMoveWithCollision(self, moveDelta);
  if (!wasNoPush) {
    ((CollisionCollider *)self->bodyCollider)->flags &= ~0x40u;
  }
  dirXZ[0] = delta[0];
  dirXZ[1] = delta[1];
  dirXZ[2] = delta[2];
  dirXZ[3] = delta[3];
  dirXZ[1] = zero;
  /* the asm tests (x | y | z) & 0x7fffffff == 0: every lane ±0 */
  if (dirXZ[0] == 0.0f && dirXZ[1] == 0.0f && dirXZ[2] == 0.0f) {
    return;
  }
  if (lenSq <= 0.001f) {
    return;
  }
  /* dirXZ, accum = normalised and clamped to [-1, 1] per lane; w lane masked (register holds S713 = 0) */
  len = dirXZ[0] * dirXZ[0] + dirXZ[1] * dirXZ[1] + dirXZ[2] * dirXZ[2];
  inv = (len == 0.0f) ? 0.0f : VfRsq(len);
  dirXZ[0] = VfSat1(dirXZ[0] * inv);
  dirXZ[1] = VfSat1(dirXZ[1] * inv);
  dirXZ[2] = VfSat1(dirXZ[2] * inv);
  dirXZ[3] = 0.0f;
  len = accum[0] * accum[0] + accum[1] * accum[1] + accum[2] * accum[2];
  inv = (len == 0.0f) ? 0.0f : VfRsq(len);
  accum[0] = VfSat1(accum[0] * inv);
  accum[1] = VfSat1(accum[1] * inv);
  accum[2] = VfSat1(accum[2] * inv);
  accum[3] = 0.0f;
  cosv = dirXZ[0] * accum[0] + dirXZ[1] * accum[1] + dirXZ[2] * accum[2];
  if (!(cosv < zero)) {
    return;
  }
  scale = 1.0f / (__builtin_sqrtf(lenSq) * cosv * -6.0f + 1.0f);
  /* delta *= scale (xyz); the sv.q writes lane 3 from S713 = 0 */
  delta[0] = delta[0] * scale;
  delta[1] = delta[1] * scale;
  delta[2] = delta[2] * scale;
  delta[3] = 0.0f;
}
