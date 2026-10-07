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

  r.x = m->x.x * p[0] + m->y.x * p[1] + m->z.x * p[2] + m->w.x;
  r.y = m->x.y * p[0] + m->y.y * p[1] + m->z.y * p[2] + m->w.y;
  r.z = m->x.z * p[0] + m->y.z * p[1] + m->z.z * p[2] + m->w.z;
  r.w = m->x.w * p[0] + m->y.w * p[1] + m->z.w * p[2] + m->w.w;
  return r;
}

/* One column of vmmul.q M000, M100 (= a), M200 (= b): sum over k of b[k] * column k of a */
static inline ScePspFVector4 CapsuleMulColumn(const ScePspFMatrix4 *a, const ScePspFVector4 *b)
{
  ScePspFVector4 r;

  r.x = b->x * a->x.x + b->y * a->y.x + b->z * a->z.x + b->w * a->w.x;
  r.y = b->x * a->x.y + b->y * a->y.y + b->z * a->z.y + b->w * a->w.y;
  r.z = b->x * a->x.z + b->y * a->y.z + b->z * a->z.z + b->w * a->w.z;
  r.w = b->x * a->x.w + b->y * a->y.w + b->z * a->z.w + b->w * a->w.w;
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
  axisWorld[0] = world->x.x * capsule->axis[0] + world->y.x * capsule->axis[1] + world->z.x * capsule->axis[2];
  axisWorld[1] = world->x.y * capsule->axis[0] + world->y.y * capsule->axis[1] + world->z.y * capsule->axis[2];
  axisWorld[2] = world->x.z * capsule->axis[0] + world->y.z * capsule->axis[1] + world->z.z * capsule->axis[2];
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
  ring->colour = (u32)VfI2uc(VfF2iz(VfSat0(colour->x) * 255.0f, 23)) |
                 (u32)VfI2uc(VfF2iz(VfSat0(colour->y) * 255.0f, 23)) << 8 |
                 (u32)VfI2uc(VfF2iz(VfSat0(colour->z) * 255.0f, 23)) << 16 |
                 (u32)VfI2uc(VfF2iz(VfSat0(colour->w) * 255.0f, 23)) << 24;
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
    lenSq = capsule->axis[0] * capsule->axis[0] + capsule->axis[1] * capsule->axis[1] +
            capsule->axis[2] * capsule->axis[2];
    inv = lenSq == 0.0f ? 0.0f : VfRsq(lenSq);
    zAxis.x = VfSat1(capsule->axis[0] * inv);
    zAxis.y = VfSat1(capsule->axis[1] * inv);
    zAxis.z = VfSat1(capsule->axis[2] * inv);
    xAxis.x = g_vecDown.y * zAxis.z - g_vecDown.z * zAxis.y;
    xAxis.y = g_vecDown.z * zAxis.x - g_vecDown.x * zAxis.z;
    xAxis.z = g_vecDown.x * zAxis.y - g_vecDown.y * zAxis.x;
    lenSq = xAxis.x * xAxis.x + xAxis.y * xAxis.y + xAxis.z * xAxis.z;
    inv = lenSq == 0.0f ? 0.0f : VfRsq(lenSq);
    xAxis.x = VfSat1(xAxis.x * inv);
    xAxis.y = VfSat1(xAxis.y * inv);
    xAxis.z = VfSat1(xAxis.z * inv);
    yAxis.x = zAxis.y * xAxis.z - zAxis.z * xAxis.y;
    yAxis.y = zAxis.z * xAxis.x - zAxis.x * xAxis.z;
    yAxis.z = zAxis.x * xAxis.y - zAxis.y * xAxis.x;
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
  ring->mtx.x.x = ring->mtx.x.x * radius;
  ring->mtx.x.y = ring->mtx.x.y * radius;
  ring->mtx.x.z = ring->mtx.x.z * radius;
  ring->mtx.x.w = ring->mtx.x.w * radius;
  ring->mtx.y.x = ring->mtx.y.x * radius;
  ring->mtx.y.y = ring->mtx.y.y * radius;
  ring->mtx.y.z = ring->mtx.y.z * radius;
  ring->mtx.y.w = ring->mtx.y.w * radius;
  ring->mtx.z.x = ring->mtx.z.x * radius;
  ring->mtx.z.y = ring->mtx.z.y * radius;
  ring->mtx.z.z = ring->mtx.z.z * radius;
  ring->mtx.z.w = ring->mtx.z.w * radius;
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
  endRing->mtx.w.x = endRing->mtx.w.x + axisWorld[0];
  endRing->mtx.w.y = endRing->mtx.w.y + axisWorld[1];
  endRing->mtx.w.z = endRing->mtx.w.z + axisWorld[2];
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
