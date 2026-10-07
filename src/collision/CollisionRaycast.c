// bdc 0x0881a7fc CollisionRaycast
#include "bdc.h"

/* Tests a ray/shape query against every active collider of the world and returns the last collider
   that was hit (NULL if none). Colliders are the nodes of g_collisionRaycastColliders (followed
   through node.next); colliders with flags bit 1 or 2 set are skipped, and a collider is only
   tested when `1 << (layer & 31)` is set in `layerMask`. Resets the nearest-hit distances of
   g_collisionHitInfo to +inf (g_collisionRaycastInf) and sets the face filter from `flags` first. */

void *CollisionRaycast(u32 layerMask, void *query, s32 flags)

{
  CollisionCollider *cur;
  CollisionCollider *hit;
  float inf;
  float scratch[4];

  cur = (CollisionCollider *)g_collisionRaycastColliders.head;
  inf = g_collisionRaycastInf;
  g_collisionHitInfo.bestDist = inf;
  g_collisionHitInfo.bestT = inf;
  hit = (CollisionCollider *)0;
  CollisionSetFaceFilter(flags);
  while (cur != (CollisionCollider *)0) {
    if ((cur->flags & 6) == 0 && ((1 << (cur->layer & 0x1f)) & layerMask) != 0) {
      if (CollisionTestShape(query, cur->shapeBlock, scratch, layerMask) != 0) {
        hit = cur;
      }
    }
    cur = (CollisionCollider *)cur->node.next;
  }
  return hit;
}
