// bdc 0x089efee4 CollisionDebugAddRingYZ
#include "bdc.h"

/* Adds a ring debug primitive (0xa0-byte `CoreObject` allocated from the low end of the heap, built
   by `CollisionDebugPrimCtor`) of kind 3 (circle in the second principal plane, vertex table
   `0x08aa3838`): colour packed from `colour` (each lane clamped to [0, 1], scaled by 255 (bank S701),
   converted to a byte, RGBA8 with `x` in the low byte), matrix = identity scaled by `r` on the
   diagonal with translation `centre` (`w` forced to 1.0), then `mtx * matrix` if `mtx` is non-NULL;
   sets `hasMtx`. Returns the primitive (an allocation failure is not checked: the stores go through
   NULL). */

CollisionDebugPrim *CollisionDebugAddRingYZ(float r, const ScePspFVector4 *centre, const ScePspFVector4 *colour, const ScePspFMatrix4 *mtx)
{
  bool fromLow;
  CollisionDebugPrim *mem;
  CollisionDebugPrim *prim;
  ScePspFMatrix4 b;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(CollisionDebugPrim), (char *)0x0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  prim = (CollisionDebugPrim *)0x0;
  if (mem != (CollisionDebugPrim *)0x0) {
    CollisionDebugPrimCtor(mem);
    prim = mem;
  }
  /* vsat0.q, vscl.q by S701 (255.0f), vf2iz.q 23, vi2uc.q */
  prim->colour = (u32)VfI2uc(VfF2iz(VfMul(VfSat0(colour->x), 255.0f), 23)) |
                 (u32)VfI2uc(VfF2iz(VfMul(VfSat0(colour->y), 255.0f), 23)) << 8 |
                 (u32)VfI2uc(VfF2iz(VfMul(VfSat0(colour->z), 255.0f), 23)) << 16 |
                 (u32)VfI2uc(VfF2iz(VfMul(VfSat0(colour->w), 255.0f), 23)) << 24;
  prim->kind = 3;
  prim->mtx.x.x = 1.0f; prim->mtx.x.y = 0.0f; prim->mtx.x.z = 0.0f; prim->mtx.x.w = 0.0f;
  prim->mtx.y.x = 0.0f; prim->mtx.y.y = 1.0f; prim->mtx.y.z = 0.0f; prim->mtx.y.w = 0.0f;
  prim->mtx.z.x = 0.0f; prim->mtx.z.y = 0.0f; prim->mtx.z.z = 1.0f; prim->mtx.z.w = 0.0f;
  prim->mtx.w.x = 0.0f; prim->mtx.w.y = 0.0f; prim->mtx.w.z = 0.0f; prim->mtx.w.w = 1.0f;
  prim->mtx.z.z = r;
  prim->mtx.y.y = r;
  prim->mtx.x.x = r;
  prim->mtx.w = *centre;
  prim->mtx.w.w = 1.0f;
  if (mtx != (ScePspFMatrix4 *)0x0) {
    /* vmmul.q M000, M100 (mtx), M200 (prim->mtx): column-major mtx * prim->mtx */
    b = prim->mtx;
    prim->mtx.x.x = VfDot4(mtx->x.x, mtx->y.x, mtx->z.x, mtx->w.x, b.x.x, b.x.y, b.x.z, b.x.w);
    prim->mtx.x.y = VfDot4(mtx->x.y, mtx->y.y, mtx->z.y, mtx->w.y, b.x.x, b.x.y, b.x.z, b.x.w);
    prim->mtx.x.z = VfDot4(mtx->x.z, mtx->y.z, mtx->z.z, mtx->w.z, b.x.x, b.x.y, b.x.z, b.x.w);
    prim->mtx.x.w = VfDot4(mtx->x.w, mtx->y.w, mtx->z.w, mtx->w.w, b.x.x, b.x.y, b.x.z, b.x.w);
    prim->mtx.y.x = VfDot4(mtx->x.x, mtx->y.x, mtx->z.x, mtx->w.x, b.y.x, b.y.y, b.y.z, b.y.w);
    prim->mtx.y.y = VfDot4(mtx->x.y, mtx->y.y, mtx->z.y, mtx->w.y, b.y.x, b.y.y, b.y.z, b.y.w);
    prim->mtx.y.z = VfDot4(mtx->x.z, mtx->y.z, mtx->z.z, mtx->w.z, b.y.x, b.y.y, b.y.z, b.y.w);
    prim->mtx.y.w = VfDot4(mtx->x.w, mtx->y.w, mtx->z.w, mtx->w.w, b.y.x, b.y.y, b.y.z, b.y.w);
    prim->mtx.z.x = VfDot4(mtx->x.x, mtx->y.x, mtx->z.x, mtx->w.x, b.z.x, b.z.y, b.z.z, b.z.w);
    prim->mtx.z.y = VfDot4(mtx->x.y, mtx->y.y, mtx->z.y, mtx->w.y, b.z.x, b.z.y, b.z.z, b.z.w);
    prim->mtx.z.z = VfDot4(mtx->x.z, mtx->y.z, mtx->z.z, mtx->w.z, b.z.x, b.z.y, b.z.z, b.z.w);
    prim->mtx.z.w = VfDot4(mtx->x.w, mtx->y.w, mtx->z.w, mtx->w.w, b.z.x, b.z.y, b.z.z, b.z.w);
    prim->mtx.w.x = VfDot4(mtx->x.x, mtx->y.x, mtx->z.x, mtx->w.x, b.w.x, b.w.y, b.w.z, b.w.w);
    prim->mtx.w.y = VfDot4(mtx->x.y, mtx->y.y, mtx->z.y, mtx->w.y, b.w.x, b.w.y, b.w.z, b.w.w);
    prim->mtx.w.z = VfDot4(mtx->x.z, mtx->y.z, mtx->z.z, mtx->w.z, b.w.x, b.w.y, b.w.z, b.w.w);
    prim->mtx.w.w = VfDot4(mtx->x.w, mtx->y.w, mtx->z.w, mtx->w.w, b.w.x, b.w.y, b.w.z, b.w.w);
  }
  prim->hasMtx = 1;
  return prim;
}
