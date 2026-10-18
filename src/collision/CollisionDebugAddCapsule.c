// bdc 0x089f0400 CollisionDebugAddCapsule
#include "bdc.h"

/* Adds the wireframe of a capsule (`CollisionCapsule`) as debug primitives with lifetime
   `frames`: a ring set at `start` (kinds 2, 3, 4: one new primitive built by
   `CollisionDebugPrimCtor` and two copies of it via `CollisionDebugPrimCopyCtor`), a far-end
   set (kind 2 copied from the start ring and moved by the world axis, kind 3 copied from it,
   kind 4 copied from that kind-3 copy), and four lines (`CollisionDebugAddLine`) along the
   axis from ring points of `g_capsuleDebugDirs`. The ring matrix is an orientation with
   z = axis (identity when the axis is vertical) scaled by `radius`, multiplied by `mtx`
   (`g_gfxIdentityMatrix` when NULL), with translation `mtx` * `start` (w = 1). Allocation
   failures are not checked. */

/* vtfm4.q with E100 = m and the fourth lane 1 (vfim 0x3c00): m * (p, 1) */
static inline ScePspFVector4 CapsuleTransformPoint(const ScePspFMatrix4 *m, const float *p)
{
  ScePspFVector4 r;

  r.x = VfDot4(m->x.x, m->y.x, m->z.x, m->w.x, p[0], p[1], p[2], 1.0f);
  r.y = VfDot4(m->x.y, m->y.y, m->z.y, m->w.y, p[0], p[1], p[2], 1.0f);
  r.z = VfDot4(m->x.z, m->y.z, m->z.z, m->w.z, p[0], p[1], p[2], 1.0f);
  r.w = VfDot4(m->x.w, m->y.w, m->z.w, m->w.w, p[0], p[1], p[2], 1.0f);
  return r;
}

/* One column of vmmul.q M000, M100 (= a), M200 (= b): sum over k of b[k] * column k of a */
static inline ScePspFVector4 CapsuleMulColumn(const ScePspFMatrix4 *a, const ScePspFVector4 *b)
{
  ScePspFVector4 r;

  r.x = VfDot4(b->x, b->y, b->z, b->w, a->x.x, a->y.x, a->z.x, a->w.x);
  r.y = VfDot4(b->x, b->y, b->z, b->w, a->x.y, a->y.y, a->z.y, a->w.y);
  r.z = VfDot4(b->x, b->y, b->z, b->w, a->x.z, a->y.z, a->z.z, a->w.z);
  r.w = VfDot4(b->x, b->y, b->z, b->w, a->x.w, a->y.w, a->z.w, a->w.w);
  return r;
}

void CollisionDebugAddCapsule(const CollisionCapsule *capsule, const ScePspFVector4 *colour, const ScePspFMatrix4 *mtx, s32 frames)
{
  const ScePspFMatrix4 *world;
  bool fromLow;
  bool vertical;
  CollisionDebugPrim *mem;
  CollisionDebugPrim *ring;
  CollisionDebugPrim *prim;
  CollisionDebugPrim *endRing;
  CollisionDebugPrim *line;
  s32 first;
  s32 end;
  s32 i;
  float radius;
  float lenSq;
  float inv;
  ScePspFVector4 xAxis;
  ScePspFVector4 yAxis;
  ScePspFVector4 zAxis;
  ScePspFMatrix4 product;
  float axisWorld[3];
  ScePspFVector4 startWorld;
  ScePspFVector4 lineFrom;

  if (g_capsuleDebugDirsInit == 0) {
    g_capsuleDebugDirsInit = 1;
    g_capsuleDebugDirs[0].x = 1.0f;
    g_capsuleDebugDirs[0].y = 0.0f;
    g_capsuleDebugDirs[0].z = 0.0f;
    g_capsuleDebugDirs[0].w = 1.0f;
    g_capsuleDebugDirs[1].y = 0.0f;
    g_capsuleDebugDirs[1].x = -1.0f;
    g_capsuleDebugDirs[1].z = 0.0f;
    g_capsuleDebugDirs[1].w = 1.0f;
    g_capsuleDebugDirs[2].x = 0.0f;
    g_capsuleDebugDirs[2].y = 1.0f;
    g_capsuleDebugDirs[2].z = 0.0f;
    g_capsuleDebugDirs[2].w = 1.0f;
    g_capsuleDebugDirs[3].x = 0.0f;
    g_capsuleDebugDirs[3].y = -1.0f;
    g_capsuleDebugDirs[3].z = 0.0f;
    g_capsuleDebugDirs[3].w = 1.0f;
    g_capsuleDebugDirs[4].x = 1.0f;
    g_capsuleDebugDirs[4].y = 0.0f;
    g_capsuleDebugDirs[4].z = 0.0f;
    g_capsuleDebugDirs[4].w = 1.0f;
    g_capsuleDebugDirs[5].x = -1.0f;
    g_capsuleDebugDirs[5].y = 0.0f;
    g_capsuleDebugDirs[5].z = 0.0f;
    g_capsuleDebugDirs[5].w = 1.0f;
    g_capsuleDebugDirs[6].x = 0.0f;
    g_capsuleDebugDirs[6].y = 0.0f;
    g_capsuleDebugDirs[6].z = -1.0f;
    g_capsuleDebugDirs[6].w = 1.0f;
    g_capsuleDebugDirs[7].x = 0.0f;
    g_capsuleDebugDirs[7].y = 0.0f;
    g_capsuleDebugDirs[7].z = 1.0f;
    g_capsuleDebugDirs[7].w = 1.0f;
  }
  first = 0;
  world = &g_gfxIdentityMatrix;
  if (mtx != (ScePspFMatrix4 *)0x0) {
    world = mtx;
  }
  /* vtfm3.t: 3x3 of world * axis (the stored w lane is stale and never read) */
  axisWorld[0] = VfDot3(world->x.x, world->y.x, world->z.x, capsule->axis[0], capsule->axis[1], capsule->axis[2]);
  axisWorld[1] = VfDot3(world->x.y, world->y.y, world->z.y, capsule->axis[0], capsule->axis[1], capsule->axis[2]);
  axisWorld[2] = VfDot3(world->x.z, world->y.z, world->z.z, capsule->axis[0], capsule->axis[1], capsule->axis[2]);
  startWorld = CapsuleTransformPoint(world, capsule->start);

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(CollisionDebugPrim), (char *)0x0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  ring = (CollisionDebugPrim *)0x0;
  if (mem != (CollisionDebugPrim *)0x0) {
    CollisionDebugPrimCtor(mem);
    ring = mem;
  }
  /* vsat0, scale by S701 (255), vf2iz 23, vi2uc: one byte per lane, x in the low byte */
  ring->colour = (u32)VfI2uc(VfF2iz(VfMul(VfSat0(colour->x), 255.0f), 23)) |
                 (u32)VfI2uc(VfF2iz(VfMul(VfSat0(colour->y), 255.0f), 23)) << 8 |
                 (u32)VfI2uc(VfF2iz(VfMul(VfSat0(colour->z), 255.0f), 23)) << 16 |
                 (u32)VfI2uc(VfF2iz(VfMul(VfSat0(colour->w), 255.0f), 23)) << 24;
  ring->kind = 2;

  vertical = false;
  if (capsule->axis[0] * capsule->axis[0] + capsule->axis[2] * capsule->axis[2] < 1e-05f) {
    if (!(capsule->axis[1] * capsule->axis[1] <= 0.0001f)) {
      vertical = true;
    }
  }
  if (vertical) {
    first = 4;
    ring->mtx.x.x = 1.0f;
    ring->mtx.x.y = 0.0f;
    ring->mtx.x.z = 0.0f;
    ring->mtx.x.w = 0.0f;
    ring->mtx.y.x = 0.0f;
    ring->mtx.y.y = 1.0f;
    ring->mtx.y.z = 0.0f;
    ring->mtx.y.w = 0.0f;
    ring->mtx.z.x = 0.0f;
    ring->mtx.z.y = 0.0f;
    ring->mtx.z.z = 1.0f;
    ring->mtx.z.w = 0.0f;
    ring->mtx.w.x = 0.0f;
    ring->mtx.w.y = 0.0f;
    ring->mtx.w.z = 0.0f;
    ring->mtx.w.w = 1.0f;
    end = 8;
  } else {
    /* inlined `MathMat4LookDir`(&ring->mtx, axis, &g_vecDown); a zero length scales by S713 (0) */
    lenSq = VfDot3(capsule->axis[0], capsule->axis[1], capsule->axis[2], capsule->axis[0], capsule->axis[1], capsule->axis[2]);
    inv = VfIsZero(lenSq) ? 0.0f : VfRsq(lenSq);
    zAxis.x = VfSat1(VfMul(capsule->axis[0], inv));
    zAxis.y = VfSat1(VfMul(capsule->axis[1], inv));
    zAxis.z = VfSat1(VfMul(capsule->axis[2], inv));
    xAxis.x = VfDot2(g_vecDown.y, g_vecDown.z, zAxis.z, -zAxis.y);
    xAxis.y = VfDot2(g_vecDown.z, g_vecDown.x, zAxis.x, -zAxis.z);
    xAxis.z = VfDot2(g_vecDown.x, g_vecDown.y, zAxis.y, -zAxis.x);
    lenSq = VfDot3(xAxis.x, xAxis.y, xAxis.z, xAxis.x, xAxis.y, xAxis.z);
    inv = VfIsZero(lenSq) ? 0.0f : VfRsq(lenSq);
    xAxis.x = VfSat1(VfMul(xAxis.x, inv));
    xAxis.y = VfSat1(VfMul(xAxis.y, inv));
    xAxis.z = VfSat1(VfMul(xAxis.z, inv));
    yAxis.x = VfDot2(zAxis.y, zAxis.z, xAxis.z, -xAxis.y);
    yAxis.y = VfDot2(zAxis.z, zAxis.x, xAxis.x, -xAxis.z);
    yAxis.z = VfDot2(zAxis.x, zAxis.y, xAxis.y, -xAxis.x);
    ring->mtx.x.x = xAxis.x;
    ring->mtx.x.y = xAxis.y;
    ring->mtx.x.z = xAxis.z;
    ring->mtx.x.w = 0.0f;
    ring->mtx.y.x = yAxis.x;
    ring->mtx.y.y = yAxis.y;
    ring->mtx.y.z = yAxis.z;
    ring->mtx.y.w = 0.0f;
    ring->mtx.z.x = zAxis.x;
    ring->mtx.z.y = zAxis.y;
    ring->mtx.z.z = zAxis.z;
    ring->mtx.z.w = 0.0f;
    ring->mtx.w.x = 0.0f;
    ring->mtx.w.y = 0.0f;
    ring->mtx.w.z = 0.0f;
    ring->mtx.w.w = 1.0f;
    end = 4;
  }

  /* scale the x, y, z columns (all four lanes) by the radius */
  radius = capsule->radius;
  ring->mtx.x.x = VfMul(ring->mtx.x.x, radius);
  ring->mtx.x.y = VfMul(ring->mtx.x.y, radius);
  ring->mtx.x.z = VfMul(ring->mtx.x.z, radius);
  ring->mtx.x.w = VfMul(ring->mtx.x.w, radius);
  ring->mtx.y.x = VfMul(ring->mtx.y.x, radius);
  ring->mtx.y.y = VfMul(ring->mtx.y.y, radius);
  ring->mtx.y.z = VfMul(ring->mtx.y.z, radius);
  ring->mtx.y.w = VfMul(ring->mtx.y.w, radius);
  ring->mtx.z.x = VfMul(ring->mtx.z.x, radius);
  ring->mtx.z.y = VfMul(ring->mtx.z.y, radius);
  ring->mtx.z.z = VfMul(ring->mtx.z.z, radius);
  ring->mtx.z.w = VfMul(ring->mtx.z.w, radius);
  /* ring->mtx = world * ring->mtx (through `product`), then translation = startWorld, w = 1 */
  product.x = CapsuleMulColumn(world, &ring->mtx.x);
  product.y = CapsuleMulColumn(world, &ring->mtx.y);
  product.z = CapsuleMulColumn(world, &ring->mtx.z);
  product.w = CapsuleMulColumn(world, &ring->mtx.w);
  ring->mtx = product;
  ring->mtx.w = startWorld;
  ring->mtx.w.w = 1.0f;
  ring->frames = frames;
  ring->hasMtx = 1;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(CollisionDebugPrim), (char *)0x0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  prim = (CollisionDebugPrim *)0x0;
  if (mem != (CollisionDebugPrim *)0x0) {
    CollisionDebugPrimCopyCtor(mem, ring);
    prim = mem;
  }
  prim->kind = 3;
  prim->frames = frames;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(CollisionDebugPrim), (char *)0x0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  prim = (CollisionDebugPrim *)0x0;
  if (mem != (CollisionDebugPrim *)0x0) {
    CollisionDebugPrimCopyCtor(mem, ring);
    prim = mem;
  }
  prim->kind = 4;
  prim->frames = frames;

  /* far-end ring set: copy of the kind-2 ring moved by axisWorld */
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(CollisionDebugPrim), (char *)0x0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  endRing = (CollisionDebugPrim *)0x0;
  if (mem != (CollisionDebugPrim *)0x0) {
    CollisionDebugPrimCopyCtor(mem, ring);
    endRing = mem;
  }
  endRing->mtx.w.x = VfAdd(endRing->mtx.w.x, axisWorld[0]);
  endRing->mtx.w.y = VfAdd(endRing->mtx.w.y, axisWorld[1]);
  endRing->mtx.w.z = VfAdd(endRing->mtx.w.z, axisWorld[2]);
  endRing->mtx.w.w = 1.0f;
  endRing->frames = frames;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(CollisionDebugPrim), (char *)0x0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != (CollisionDebugPrim *)0x0) {
    CollisionDebugPrimCopyCtor(mem, endRing);
  }
  /* the original reuses the far-end slot: the kind-4 copy below is made from this kind-3 copy */
  endRing = mem;
  endRing->kind = 3;
  endRing->frames = frames;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(CollisionDebugPrim), (char *)0x0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  prim = (CollisionDebugPrim *)0x0;
  if (mem != (CollisionDebugPrim *)0x0) {
    CollisionDebugPrimCopyCtor(mem, endRing);
    prim = mem;
  }
  prim->kind = 4;
  prim->frames = frames;

  /* lines along the axis from the start-ring points */
  for (i = first; i < end; i++) {
    lineFrom = CapsuleTransformPoint(&ring->mtx, &g_capsuleDebugDirs[i].x);
    line = CollisionDebugAddLine(&lineFrom, axisWorld, colour, (ScePspFMatrix4 *)0x0);
    line->frames = frames;
  }
  return;
}
