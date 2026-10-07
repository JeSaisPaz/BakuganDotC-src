// bdc 0x088b88fc ActorBallCheckHit
#include "bdc.h"

/* Collision sweep of a flying `ActorBall` from `pos` along `vel` with `radius`. The layer mask is
   `(1 << layer of the owner's collider2) ^ 0x01bf0f7e | 0x18000000`. First fills the static query
   `g_actorBallHitQuery` (shape = the swept sphere `g_collisionSweptSphereDesc` with a doubled
   radius, owner = the ball, heading = `atan2f(vel.z, vel.x)`, `attackId`;
   `g_collisionAttackFlags` = `attackFlags`) and runs `CollisionHitQuery`, then restores the sweep
   to the real radius. With no hit it raycasts the map (`CollisionRaycast` with the sweep block); on
   a map hit it sets `flag1d0`, copies the hit point (`g_collisionHitResult`) to the query `contact`
   and `vec1e0`, clears `hitCollider`, stores the surface (`unk190`) and the `hitNormal`, sets
   `hitType` 1 and returns 1 (0 when nothing was hit). On a query hit it calls
   `ActorNpcSwitchRobotCheckSwitchHit` for the first actor whose vtable slot 11 passes and whose
   `collider2` is the hit collider, records the ball position in `vec1e0` with `hitType` 2 (3 when the
   hit collider's layer is 0x13, taking its owner's position if any), and, unless the player is
   scanning (`scan`), cancels the hit (`hitType` 0, returns 0) when the collider is the `attached`
   collider of a field gimmick whose slot 17 passes. Otherwise returns the `CollisionHitQuery`
   result. */

s32 ActorBallCheckHit(float radius, ActorBall *ball, float *pos, float *vel, s32 attackId, s32 attackFlags)
{
  CollisionQuery *query;
  CollisionSweptSphereDesc *desc;
  CollisionShapeBlock *block;
  CollisionCollider *hit;
  GfxModel *hitOwner;
  Actor *actor;
  ActorPlayer *player;
  GameGimmick *gimmick;
  const VtblEntry *e;
  u32 layerMask;
  s32 result;

  layerMask = ((1u << ((CollisionCollider *)((Actor *)ball->owner)->collider2)->layer) ^ 0x01bf0f7eu) |
              0x08000000u | 0x10000000u;
  desc = &g_collisionSweptSphereDesc;
  block = (CollisionShapeBlock *)desc->shapeBlock;
  query = &g_actorBallHitQuery;

  /* Sweep with twice the radius for the object query. */
  desc->start = *(const ScePspFVector4 *)pos;
  desc->dir = *(const ScePspFVector4 *)vel;
  desc->radius = radius * 2.0f;
  desc->start.w = desc->radius * desc->radius;
  /* dir.w = |dir.xyz| */
  desc->dir.w = __builtin_sqrtf(desc->dir.x * desc->dir.x + desc->dir.y * desc->dir.y + desc->dir.z * desc->dir.z);
  query->shape = block;
  query->owner = ball;
  query->heading = atan2f(vel[2], vel[0]);
  query->attackId = attackId;
  ((CollisionSweptSphereDesc *)block->shape)->start = *(const ScePspFVector4 *)pos;
  g_collisionAttackFlags = attackFlags;
  result = CollisionHitQuery(layerMask, (float *)query, 1, NULL);

  /* Restore the sweep to the real radius. */
  desc->start = *(const ScePspFVector4 *)pos;
  desc->dir = *(const ScePspFVector4 *)vel;
  desc->radius = radius;
  desc->start.w = radius * radius;
  desc->dir.w = __builtin_sqrtf(desc->dir.x * desc->dir.x + desc->dir.y * desc->dir.y + desc->dir.z * desc->dir.z);

  if (result == 0) {
    /* No object hit: raycast the map. */
    if (CollisionRaycast(layerMask, block, 0) != NULL) {
      result = 1;
      ball->flag1d0 = 1;
      query->contact = g_collisionHitResult.point;
      *(ScePspFVector4 *)ball->vec1e0 = query->contact;
      query->hitCollider = NULL;
      ball->unk190 = g_collisionHitInfo.surface;
      *(ScePspFVector4 *)ball->hitNormal = g_collisionHitResult.normal;
      ball->hitType = 1;
    }
    return result;
  }

  /* Object hit: let a matching switch robot react. */
  if (query->hitCollider != NULL) {
    for (actor = *(Actor **)ActorGetList(); actor != NULL; actor = (Actor *)actor->base.base.next) {
      e = &((const VtblEntry *)actor->base.base.vtable)[11];
      if (((int (*)(void *))e->fn)((char *)actor + e->delta) == 0) {
        continue;
      }
      if (query->hitCollider == actor->collider2) {
        ActorNpcSwitchRobotCheckSwitchHit((ActorNpcSwitchRobot *)actor, &query->contact.x);
        break;
      }
    }
  }
  *(ScePspFVector4 *)ball->vec1e0 = *(const ScePspFVector4 *)ball->base.pos;
  ball->hitType = 2;
  hit = (CollisionCollider *)query->hitCollider;
  if (hit != NULL && hit->layer == 0x13) {
    ball->hitType = 3;
    hitOwner = (GfxModel *)((CollisionCollider *)query->hitCollider)->owner;
    if (hitOwner != NULL) {
      *(ScePspFVector4 *)ball->vec1e0 = *(const ScePspFVector4 *)hitOwner->pos;
    }
  }
  /* Hits on a field gimmick's attached collider do not count (unless the player is scanning). */
  if (query->hitCollider != NULL) {
    player = (ActorPlayer *)ActorFindPlayer();
    if (player != NULL && player->scan == 0) {
      for (gimmick = ((GameFieldTask *)GameFieldFindTask())->gimmicks; gimmick != NULL;
           gimmick = (GameGimmick *)gimmick->base.base.next) {
        e = &((const VtblEntry *)gimmick->base.base.vtable)[17];
        if (((int (*)(void *))e->fn)((char *)gimmick + e->delta) == 0) {
          continue;
        }
        if (gimmick->attached == NULL) {
          continue;
        }
        if (query->hitCollider == gimmick->attached) {
          ball->hitType = 0;
          result = 0;
        }
      }
    }
  }
  return result;
}
