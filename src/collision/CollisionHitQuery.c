// bdc 0x0881a9e8 CollisionHitQuery
#include "bdc.h"

/* Tests the hit query `query` (`CollisionQuery`) against the colliders of
   `g_collisionHitColliders` and then `g_collisionRaycastColliders` (the second group is only
   walked when the first is not empty, and only its colliders with `byte180` set are tested).
   A collider is skipped when it is the query's own collider, that collider's `ignoreCollider`,
   or `exclude`; when its layer bit (`1 << layer`) is not in `layerMask`; when its flags hit the
   skip mask (0x13, or 2 when `soft`) unless the attack id is 24; or when it has flag 0x200 and
   the attack id is 22. Each overlap (`CollisionTestShape`) stores the contact point and the
   collider in the query, then by attack id:
   - 24: returns 1 at once;
   - 0xbb on a collider with flags exactly 0x13 or layer 11: writes the hit into the collider
     (below), moves it to layer 0x13 and returns 1;
   - 26..31 on a collider whose owner is a live unit (`BtlBakuganListFind`): the collider is
     ignored when the unit's vtable entry 11 answers non-zero or entry 20 equals `id - 26`.
   Otherwise a collider without any of the flags 0x13 gets the hit: flag 0x10, `hitPos`,
   `hitType`/`hitKind`/`hitHeading`/`hitAttacker`/`hitParam164` from the query and
   `g_collisionAttackFlags` as `hitDuration`; a guarding collider (flag 8) sets
   `g_collisionLastHitGuarded` and `g_collisionLastHitUnit`, and ids 0x3f / 0x1f take the
   contact x/z from the query shape's start point. A collider with any of the flags 0x13 is only
   recorded in `g_collisionLastHitCollider` and the query's hit collider goes back to the
   previous one. With query flag bit 0 the collider's `hitHeading` is recomputed from the query
   shape centre (shape vtable entry 8) to the contact. The query then counts as hit; id 22 returns
   at once, otherwise the owner collider (if any, and the id is not 25) gets the attack flags as
   `cooldown` and the walk goes on. Returns 1 when anything was hit, else 0. */

static inline ScePspFVector4 *CollisionHitQueryShapeCenter(CollisionSweptSphereDesc *desc)
{
  const VtblEntry *entry = &desc->vtbl[8];

  return ((ScePspFVector4 * (*)(void *)) entry->fn)((char *)desc + entry->delta);
}

char CollisionHitQuery(u32 layerMask, float *query, char soft, void *exclude)

{
  CollisionQuery *q = (CollisionQuery *)query;
  CollisionCollider *col = (CollisionCollider *)g_collisionHitColliders.head;
  CollisionCollider *nextList = (CollisionCollider *)g_collisionRaycastColliders.head;
  CollisionCollider *ignore = NULL;
  CollisionCollider *prevHit;
  CollisionSweptSphereDesc *desc;
  BtlBakugan *unit;
  const VtblEntry *vtbl;
  ScePspFVector4 *center;
  ScePspFVector4 contact;
  ScePspFVector4 pos;
  u32 skipFlags;
  s32 firstList = 1;
  char result = 0;

  CollisionSetFaceFilter(0);
  q->hitCollider = NULL;
  if (q->ownerCollider != NULL) {
    ignore = ((CollisionCollider *)q->ownerCollider)->ignoreCollider;
  }
  g_collisionLastHitGuarded = 0;
  g_collisionLastHitCollider = NULL;
  skipFlags = 0x13;
  if (soft != 0) {
    skipFlags = 2;
  }
  g_collisionHitInfo.bestDist = __builtin_inff();
  g_collisionHitInfo.bestT = __builtin_inff();

  while (col != NULL) {
    if (col == q->ownerCollider || col == ignore) {
      goto next;
    }
    if ((col->flags & skipFlags) != 0 && q->attackId != 24) {
      goto next;
    }
    if (((1 << col->layer) & layerMask) == 0) {
      goto next;
    }
    if (!firstList && col->byte180 == 0) {
      goto next;
    }
    if (col == exclude) {
      goto next;
    }
    if ((col->flags & 0x200) != 0 && q->attackId == 22) {
      goto next;
    }
    if (CollisionTestShape(q->shape, col->shapeBlock, &contact.x, layerMask) == 0) {
      goto next;
    }

    prevHit = (CollisionCollider *)q->hitCollider;
    q->hitCollider = col;
    q->contact = contact;
    if (q->attackId == 24) {
      result = 1;
      break;
    }
    if (q->attackId == 0xbb && (col->flags == 0x13 || col->layer == 0xb)) {
      col->flags |= 0x10;
      col->hitPos = q->contact;
      col->hitType = q->attackSide;
      col->hitKind = q->attackId;
      col->hitHeading = q->heading;
      col->hitAttacker = q->owner;
      col->hitParam164 = q->hitParam;
      col->hitDuration = g_collisionAttackFlags;
      q->hitCollider = col;
      col->layer = 0x13;
      result = 1;
      break;
    }
    if (q->attackId >= 26 && q->attackId < 32 && col->owner != NULL) {
      unit = (BtlBakugan *)BtlBakuganListFind((BtlBakugan *)col->owner);
      if (unit != NULL) {
        vtbl = (const VtblEntry *)unit->base.base.vtable;
        if (((s32 (*)(void *))vtbl[11].fn)((u8 *)unit + vtbl[11].delta) != 0) {
          goto next;
        }
        vtbl = (const VtblEntry *)unit->base.base.vtable;
        if (((s32 (*)(void *))vtbl[20].fn)((u8 *)unit + vtbl[20].delta) == q->attackId - 26) {
          goto next;
        }
      }
    }

    if ((col->flags & 0x13) == 0) {
      col->flags |= 0x10;
      col->hitPos = q->contact;
      col->hitType = q->attackSide;
      col->hitKind = q->attackId;
      col->hitHeading = q->heading;
      col->hitAttacker = q->owner;
      col->hitParam164 = q->hitParam;
      col->hitDuration = g_collisionAttackFlags;
      if ((col->flags & 8) != 0) {
        g_collisionLastHitGuarded = 1;
        g_collisionLastHitUnit = (BtlBakugan *)q->owner;
      }
      if (q->attackId == 0x3f || q->attackId == 0x1f) {
        desc = (CollisionSweptSphereDesc *)((CollisionShapeBlock *)q->shape)->desc;
        col->hitPos.x = desc->start.x;
        desc = (CollisionSweptSphereDesc *)((CollisionShapeBlock *)q->shape)->desc;
        col->hitPos.z = desc->start.z;
      }
    } else {
      g_collisionLastHitCollider = col;
      q->hitCollider = prevHit;
    }
    if ((q->flags & 1) != 0) {
      center = CollisionHitQueryShapeCenter(
          (CollisionSweptSphereDesc *)((CollisionShapeBlock *)q->shape)->desc);
      pos = q->contact;
      col->hitHeading = atan2f(pos.z - center->z, pos.x - center->x);
    }
    result = 1;
    if (q->attackId == 22) {
      break;
    }
    if (q->ownerCollider != NULL && q->attackId != 0x19) {
      ((CollisionCollider *)q->ownerCollider)->cooldown = g_collisionAttackFlags;
    }

  next:
    col = (CollisionCollider *)col->node.next;
    if (col == NULL) {
      col = nextList;
      firstList = 0;
      nextList = NULL;
    }
  }
  return result;
}
