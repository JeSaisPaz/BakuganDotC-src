// bdc 0x089ea220 CollisionBoxTestSegmentQuery
#include "bdc.h"

/* Tests an oriented box collider against a segment query: caches the inverse transform of the box
   (`invTransform`, flag `invValid`), transforms the query into box space through its vtable (slot
   `+0x34`) into the static segment shape `g_collisionBoxSegQueryShape`, then returns
   `CollisionAabbOverlapSegment` of the box extents (`aabbMin`/`aabbMax`) against it. The inverse
   is the transposed rotation (rows `w` 0) with the negated rotated translation; its `w.w` is the
   transform's `w.w`, as the listing leaves it. */

bool CollisionBoxTestSegmentQuery(CollisionBox *self, void *query)

{
  const VtblEntry *vtbl;
  const ScePspFMatrix4 *t;
  ScePspFMatrix4 *inv;
  float px;
  float py;
  float pz;

  if (g_collisionBoxSegQueryInit == 0) {
    g_collisionBoxSegQueryInit = 1;
    g_collisionBoxSegQueryShape.info = g_collisionSegmentVtbl;
    g_collisionBoxSegQueryShape.type = 2;
  }
  if (self->invValid == 0) {
    t = &self->transform;
    inv = &self->invTransform;
    px = t->x.x * t->w.x + t->x.y * t->w.y + t->x.z * t->w.z;
    py = t->y.x * t->w.x + t->y.y * t->w.y + t->y.z * t->w.z;
    pz = t->z.x * t->w.x + t->z.y * t->w.y + t->z.z * t->w.z;
    inv->x.x = t->x.x;
    inv->x.y = t->y.x;
    inv->x.z = t->z.x;
    inv->x.w = 0.0f;
    inv->y.x = t->x.y;
    inv->y.y = t->y.y;
    inv->y.z = t->z.y;
    inv->y.w = 0.0f;
    inv->z.x = t->x.z;
    inv->z.y = t->y.z;
    inv->z.z = t->z.z;
    inv->z.w = 0.0f;
    inv->w.x = -px;
    inv->w.y = -py;
    inv->w.z = -pz;
    inv->w.w = t->w.w;
    self->invValid = 1;
  }
  vtbl = ((const CollisionBox *)query)->vtbl + 6;
  ((void (*)(void *, const ScePspFMatrix4 *, SegmentShape *))vtbl->fn)(
      (char *)query + vtbl->delta, &self->invTransform, &g_collisionBoxSegQueryShape);
  return CollisionAabbOverlapSegment(&(self->aabbMin).x, &g_collisionBoxSegQueryShape);
}
