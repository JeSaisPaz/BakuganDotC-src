// bdc 0x089f0230 CollisionDebugAddBox
#include "bdc.h"

/* Adds a box wireframe debug primitive (0xa0-byte `CoreObject` allocated from the low end of the
   heap, built by `CollisionDebugPrimCtor`) of kind 5 spanning `min`..`max`: colour packed from
   `colour` (each lane clamped to [0, 1], scaled by 255 (bank S701), converted to a byte, RGBA8 with
   `x` in the low byte), matrix = identity with the diagonal set to `max - min` and translation
   `(min + max) * 0.5` (`w` forced to 1.0), then `mtx * matrix` if `mtx` is non-NULL; sets `hasMtx`.
   Returns the primitive (an allocation failure is not checked: the stores go through NULL). */

CollisionDebugPrim *CollisionDebugAddBox(const float *min, const float *max, const ScePspFVector4 *colour, const ScePspFMatrix4 *mtx)
{
  bool fromLow;
  CollisionDebugPrim *mem;
  CollisionDebugPrim *prim;
  ScePspFMatrix4 b;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0xa0, (char *)0x0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  prim = (CollisionDebugPrim *)0x0;
  if (mem != (CollisionDebugPrim *)0x0) {
    CollisionDebugPrimCtor(mem);
    prim = mem;
  }
  /* vsat0.q, vscl.q by S701 (255.0f), vf2iz.q 23, vi2uc.q */
  prim->colour = (u32)VfI2uc(VfF2iz(VfSat0(colour->x) * 255.0f, 23)) |
                 (u32)VfI2uc(VfF2iz(VfSat0(colour->y) * 255.0f, 23)) << 8 |
                 (u32)VfI2uc(VfF2iz(VfSat0(colour->z) * 255.0f, 23)) << 16 |
                 (u32)VfI2uc(VfF2iz(VfSat0(colour->w) * 255.0f, 23)) << 24;
  prim->kind = 5;
  /* vmidt.q, then the diagonal = max - min (vsub.t) */
  prim->mtx.x.x = 1.0f; prim->mtx.x.y = 0.0f; prim->mtx.x.z = 0.0f; prim->mtx.x.w = 0.0f;
  prim->mtx.y.x = 0.0f; prim->mtx.y.y = 1.0f; prim->mtx.y.z = 0.0f; prim->mtx.y.w = 0.0f;
  prim->mtx.z.x = 0.0f; prim->mtx.z.y = 0.0f; prim->mtx.z.z = 1.0f; prim->mtx.z.w = 0.0f;
  prim->mtx.w.x = 0.0f; prim->mtx.w.y = 0.0f; prim->mtx.w.z = 0.0f; prim->mtx.w.w = 1.0f;
  prim->mtx.x.x = max[0] - min[0];
  prim->mtx.y.y = max[1] - min[1];
  prim->mtx.z.z = max[2] - min[2];
  /* translation = (min + max) * 0.5 (vadd.t, vscl.t into C710; lane w = S713 = 0, then 1.0) */
  prim->mtx.w.x = (min[0] + max[0]) * 0.5f;
  prim->mtx.w.y = (min[1] + max[1]) * 0.5f;
  prim->mtx.w.z = (min[2] + max[2]) * 0.5f;
  prim->mtx.w.w = 1.0f;
  if (mtx != (ScePspFMatrix4 *)0x0) {
    /* vmmul.q M000, M100 (mtx), M200 (prim->mtx): column-major mtx * prim->mtx */
    b = prim->mtx;
    prim->mtx.x.x = mtx->x.x * b.x.x + mtx->y.x * b.x.y + mtx->z.x * b.x.z + mtx->w.x * b.x.w;
    prim->mtx.x.y = mtx->x.y * b.x.x + mtx->y.y * b.x.y + mtx->z.y * b.x.z + mtx->w.y * b.x.w;
    prim->mtx.x.z = mtx->x.z * b.x.x + mtx->y.z * b.x.y + mtx->z.z * b.x.z + mtx->w.z * b.x.w;
    prim->mtx.x.w = mtx->x.w * b.x.x + mtx->y.w * b.x.y + mtx->z.w * b.x.z + mtx->w.w * b.x.w;
    prim->mtx.y.x = mtx->x.x * b.y.x + mtx->y.x * b.y.y + mtx->z.x * b.y.z + mtx->w.x * b.y.w;
    prim->mtx.y.y = mtx->x.y * b.y.x + mtx->y.y * b.y.y + mtx->z.y * b.y.z + mtx->w.y * b.y.w;
    prim->mtx.y.z = mtx->x.z * b.y.x + mtx->y.z * b.y.y + mtx->z.z * b.y.z + mtx->w.z * b.y.w;
    prim->mtx.y.w = mtx->x.w * b.y.x + mtx->y.w * b.y.y + mtx->z.w * b.y.z + mtx->w.w * b.y.w;
    prim->mtx.z.x = mtx->x.x * b.z.x + mtx->y.x * b.z.y + mtx->z.x * b.z.z + mtx->w.x * b.z.w;
    prim->mtx.z.y = mtx->x.y * b.z.x + mtx->y.y * b.z.y + mtx->z.y * b.z.z + mtx->w.y * b.z.w;
    prim->mtx.z.z = mtx->x.z * b.z.x + mtx->y.z * b.z.y + mtx->z.z * b.z.z + mtx->w.z * b.z.w;
    prim->mtx.z.w = mtx->x.w * b.z.x + mtx->y.w * b.z.y + mtx->z.w * b.z.z + mtx->w.w * b.z.w;
    prim->mtx.w.x = mtx->x.x * b.w.x + mtx->y.x * b.w.y + mtx->z.x * b.w.z + mtx->w.x * b.w.w;
    prim->mtx.w.y = mtx->x.y * b.w.x + mtx->y.y * b.w.y + mtx->z.y * b.w.z + mtx->w.y * b.w.w;
    prim->mtx.w.z = mtx->x.z * b.w.x + mtx->y.z * b.w.y + mtx->z.z * b.w.z + mtx->w.z * b.w.w;
    prim->mtx.w.w = mtx->x.w * b.w.x + mtx->y.w * b.w.y + mtx->z.w * b.w.z + mtx->w.w * b.w.w;
  }
  prim->hasMtx = 1;
  return prim;
}
