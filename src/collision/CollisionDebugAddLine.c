// bdc 0x089efc40 CollisionDebugAddLine
#include "bdc.h"

/* Adds a line debug primitive (0xa0-byte `CoreObject` allocated from the low end of the heap, built
   by `CollisionDebugPrimCtor`, kind left at 0 = line): without `mtx`, `from` = `origin` and
   `to` = `origin` + `dir` (xyz; `w` copied from `origin`); with `mtx`, `from` = `mtx` * `origin`
   (4x4, column-major) and `to` = `from` + the upper 3x3 of `mtx` * `dir` (xyz; `w` from `from`).
   Colour packed from `colour` (each lane clamped to [0, 1], scaled by 255 (bank S701), converted to
   a byte, RGBA8 with `x` in the low byte). Returns the primitive (an allocation failure is not
   checked: the stores go through NULL). `dir` is read as 3 floats (the listing loads 4 with
   `lv.q`, the 4th unused). */

CollisionDebugPrim *CollisionDebugAddLine(const ScePspFVector4 *origin, const float *dir, const ScePspFVector4 *colour, const ScePspFMatrix4 *mtx)
{
  bool fromLow;
  CollisionDebugPrim *mem;
  CollisionDebugPrim *prim;
  ScePspFVector4 from;
  ScePspFVector4 dirMtx;

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
  if (mtx == (ScePspFMatrix4 *)0x0) {
    prim->from = *origin;
    /* vadd.t: xyz summed, w of origin kept */
    prim->to.x = origin->x + dir[0];
    prim->to.y = origin->y + dir[1];
    prim->to.z = origin->z + dir[2];
    prim->to.w = origin->w;
  } else {
    /* vtfm4.q C000, E100, C200: mtx * origin */
    from.x = mtx->x.x * origin->x + mtx->y.x * origin->y + mtx->z.x * origin->z + mtx->w.x * origin->w;
    from.y = mtx->x.y * origin->x + mtx->y.y * origin->y + mtx->z.y * origin->z + mtx->w.y * origin->w;
    from.z = mtx->x.z * origin->x + mtx->y.z * origin->y + mtx->z.z * origin->z + mtx->w.z * origin->w;
    from.w = mtx->x.w * origin->x + mtx->y.w * origin->y + mtx->z.w * origin->z + mtx->w.w * origin->w;
    prim->from = from;
    /* vtfm3.t C000, E100, C200: upper 3x3 of mtx * dir */
    dirMtx.x = mtx->x.x * dir[0] + mtx->y.x * dir[1] + mtx->z.x * dir[2];
    dirMtx.y = mtx->x.y * dir[0] + mtx->y.y * dir[1] + mtx->z.y * dir[2];
    dirMtx.z = mtx->x.z * dir[0] + mtx->y.z * dir[1] + mtx->z.z * dir[2];
    /* vadd.t: xyz summed, w of from kept */
    prim->to.x = from.x + dirMtx.x;
    prim->to.y = from.y + dirMtx.y;
    prim->to.z = from.z + dirMtx.z;
    prim->to.w = from.w;
  }
  /* vsat0.q, vscl.q by S701 (255.0f), vf2iz.q 23, vi2uc.q */
  prim->colour = (u32)VfI2uc(VfF2iz(VfSat0(colour->x) * 255.0f, 23)) |
                 (u32)VfI2uc(VfF2iz(VfSat0(colour->y) * 255.0f, 23)) << 8 |
                 (u32)VfI2uc(VfF2iz(VfSat0(colour->z) * 255.0f, 23)) << 16 |
                 (u32)VfI2uc(VfF2iz(VfSat0(colour->w) * 255.0f, 23)) << 24;
  return prim;
}
