// bdc 0x0881a5c8 CollisionTestPair
#include "bdc.h"

/* Tests the hit query `query` (`CollisionQuery`) against one collider `other` for the layers in
   `layerMask`. Returns 0 without an owner collider. With `exact` set, colliders flagged 0x13 are
   skipped and the query shape is overlapped with the collider (`CollisionTestShape`); no overlap
   returns 0. With `exact` 0 the owner collider's own shape block is tested instead and, when they
   do not overlap, the contact falls back to the point 0.9 of the way from the owner shape's centre
   to the collider shape's centre (shape vtable entry 8), at the owner centre's height: this mode
   always hits. On a hit it fills the contact data like `CollisionHitQuery` (contact and collider
   in the query, flag 0x10 and the attack data in the collider, `g_collisionLastHitGuarded` /
   `g_collisionLastHitUnit`), recomputes the collider's hit heading from the query shape centre
   when query flag bit 0 is set, stores `g_collisionAttackFlags` in the owner collider's
   `cooldown`, and returns 1. */

static inline ScePspFVector4 *CollisionTestPairShapeCenter(CollisionShapeBlock *shape)
{
  const VtblEntry *entry = &shape->vtbl[8];

  return ((ScePspFVector4 * (*)(void *)) entry->fn)((char *)shape + entry->delta);
}

int CollisionTestPair(u32 layerMask, void *other, float *query, char exact)

{
  CollisionQuery *q = (CollisionQuery *)query;
  CollisionCollider *col = (CollisionCollider *)other;
  CollisionCollider *owner;
  CollisionShapeBlock *ownerBlock;
  ScePspFVector4 *center;
  ScePspFVector4 contact;
  ScePspFVector4 mid;
  ScePspFVector4 ownerCenter;
  ScePspFVector4 otherCenter;
  ScePspFVector4 pos;

  if (q->ownerCollider == (void *)0) {
    return 0;
  }
  if (exact == 0) {
    owner = (CollisionCollider *)q->ownerCollider;
    ownerBlock = (CollisionShapeBlock *)owner->shapeBlock;
    if (CollisionTestShape(ownerBlock, col->shapeBlock, &contact.x, layerMask) == 0) {
      center = CollisionTestPairShapeCenter((CollisionShapeBlock *)ownerBlock->shape);
      ownerCenter = *center;
      center = CollisionTestPairShapeCenter(
          (CollisionShapeBlock *)((CollisionShapeBlock *)col->shapeBlock)->shape);
      otherCenter = *center;
      /* mid = ownerCenter + (otherCenter - ownerCenter) * 0.9f, staged through `mid` */
      mid.x = ownerCenter.x + (otherCenter.x - ownerCenter.x) * 0.9f;
      mid.y = ownerCenter.y + (otherCenter.y - ownerCenter.y) * 0.9f;
      mid.z = ownerCenter.z + (otherCenter.z - ownerCenter.z) * 0.9f;
      mid.w = ownerCenter.w + (otherCenter.w - ownerCenter.w) * 0.9f;
      contact = mid;
      contact.y = ownerCenter.y;
    }
  } else {
    if ((col->flags & 0x13) != 0) {
      return 0;
    }
    if (CollisionTestShape(q->shape, col->shapeBlock, &contact.x, layerMask) == 0) {
      return 0;
    }
  }
  q->contact = contact;
  q->hitCollider = col;
  g_collisionLastHitGuarded = (col->flags & 8) != 0;
  g_collisionLastHitUnit = (BtlBakugan *)q->owner;
  col->flags |= 0x10;
  col->hitPos = q->contact;
  col->hitType = q->attackSide;
  col->hitKind = q->attackId;
  col->hitHeading = q->heading;
  col->hitAttacker = q->owner;
  col->hitParam164 = q->hitParam;
  col->hitDuration = g_collisionAttackFlags;
  if ((q->flags & 1) != 0) {
    center = CollisionTestPairShapeCenter(
        (CollisionShapeBlock *)((CollisionShapeBlock *)q->shape)->shape);
    pos = q->contact;
    col->hitHeading = atan2f(pos.z - center->z, pos.x - center->x);
  }
  if (q->ownerCollider != (void *)0) {
    ((CollisionCollider *)q->ownerCollider)->cooldown = g_collisionAttackFlags;
  }
  return 1;
}
