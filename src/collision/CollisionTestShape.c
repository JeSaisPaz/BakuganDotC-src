// bdc 0x089e562c CollisionTestShape
#include "bdc.h"

/* Tests a query shape block `query` against one collider shape block `block`: both are first
   brought up to date by `CollisionShapeBlockPrepare` (returns 0 when the query has no shape
   object). Dispatches on the block's installed shape type (`shapeType`) to the query shape's
   vtable (entry 1 type 1, 2 type 2, 3 type 3, 4 type 4, 5 type 6), called on the prepared block
   shape, or to `CollisionMeshTestShape` for type-8 meshes (`flags >> 16` = surface mask).
   A type-6 hit is copied into the global hit record (`g_collisionHitResult` point = `hit`,
   normal = `g_vecUp`, `g_collisionHitInfo` cleared). Returns non-zero on a hit, 0 for the
   other shape types. */

s32 CollisionTestShape(void *query, void *block, float *hit, u32 flags)

{
  /* The prepared shapes start with the common shape header (type, vtable). */
  CollisionShapeBlock *queryShape;
  CollisionShapeBlock *blockShape;
  const VtblEntry *entry;
  s32 result;

  queryShape = (CollisionShapeBlock *)CollisionShapeBlockPrepare(query);
  if (queryShape == (CollisionShapeBlock *)0) {
    return 0;
  }
  blockShape = (CollisionShapeBlock *)CollisionShapeBlockPrepare(block);
  switch (((CollisionShapeBlock *)block)->shapeType) {
  case 1:
    entry = &queryShape->vtbl[1];
    return ((s32 (*)(void *, void *, float *))entry->fn)((char *)queryShape + entry->delta,
                                                         blockShape, hit);
  case 2:
    entry = &queryShape->vtbl[2];
    return ((s32 (*)(void *, void *, float *))entry->fn)((char *)queryShape + entry->delta,
                                                         blockShape, hit);
  case 3:
    entry = &queryShape->vtbl[3];
    return ((s32 (*)(void *, void *, float *))entry->fn)((char *)queryShape + entry->delta,
                                                         blockShape, hit);
  case 4:
    entry = &queryShape->vtbl[4];
    return ((s32 (*)(void *, void *, float *))entry->fn)((char *)queryShape + entry->delta,
                                                         blockShape, hit);
  case 6:
    entry = &queryShape->vtbl[5];
    result = ((s32 (*)(void *, void *, float *))entry->fn)((char *)queryShape + entry->delta,
                                                           blockShape, hit);
    if (result != 0) {
      g_collisionHitInfo.bestDist = 0.0f;
      g_collisionHitResult.point = *(const ScePspFVector4 *)hit;
      g_collisionHitResult.normal = g_vecUp;
      g_collisionHitInfo.partIndex = 0;
      g_collisionHitInfo.part = (void *)0;
      g_collisionHitInfo.face = (const u16 *)0;
      g_collisionHitInfo.surface = 0;
      g_collisionHitInfo.fromClosestPoint = 0;
      g_collisionHitInfo.bestT = 0.0f;
    }
    return result;
  case 8:
    return CollisionMeshTestShape(block, queryShape, hit, flags >> 16);
  }
  return 0;
}
