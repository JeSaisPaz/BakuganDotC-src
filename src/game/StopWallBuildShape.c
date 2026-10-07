// bdc 0x088b361c StopWallBuildShape
#include "bdc.h"

/* Builds the geometry of stop wall `id` (0 or 1): reads its two corners from
   `g_stopWallCorners` (entries `id * 2` / `id * 2 + 1`), gives the wall a depth of 50 (corner A
   z - 50 for id 0, corner B z + 50 otherwise), stores them in `cornerA` / `cornerB` (w = 0), puts
   `centre` at their midpoint snapped to the floor (`CollisionRaycastPoint``(centre, centre)`),
   creates (once, 0xc0 bytes from the low heap) the type-6 `CollisionBox` `shape`
   (`g_collisionBoxVtbl`) and fills it with the two corners as `aabbMin` / `aabbMax`, the
   identity `g_gfxIdentityMatrix` as `transform` and its inverse (transposed rotation, negated
   rotated translation, w kept; `invValid = 1`). */

void StopWallBuildShape(StopWall *self, s32 id)
{
  const float *src;
  float dzA;
  float dzB;
  float a[4] __attribute__((aligned(16)));
  float b[4] __attribute__((aligned(16)));
  float mid[4] __attribute__((aligned(16)));
  CollisionBox *box;
  CollisionBox *mem;
  const ScePspFMatrix4 *m;
  ScePspFMatrix4 *inv;
  bool fromLow;
  int i;

  src = g_stopWallCorners[id * 2];
  dzA = 0.0f;
  dzB = 0.0f;
  if (id == 0) {
    dzA = -50.0f;
  } else {
    dzB = 50.0f;
  }
  a[0] = src[0];
  a[1] = src[1];
  a[2] = src[2] + dzA;
  a[3] = 0.0f;
  src = g_stopWallCorners[id * 2 + 1];
  b[0] = src[0];
  b[1] = src[1];
  b[2] = src[2] + dzB;
  b[3] = 0.0f;
  for (i = 0; i < 4; i++) {
    self->cornerA[i] = a[i];
  }
  for (i = 0; i < 4; i++) {
    self->cornerB[i] = b[i];
  }
  /* centre = A + (B - A) * 0.5 through an aligned stack temporary */
  for (i = 0; i < 4; i++) {
    mid[i] = self->cornerA[i] + (self->cornerB[i] - self->cornerA[i]) * 0.5f;
  }
  for (i = 0; i < 4; i++) {
    self->centre[i] = mid[i];
  }
  CollisionRaycastPoint(self->centre, self->centre);

  if (self->shape == NULL) {
    box = NULL;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = (CollisionBox *)MemAlloc(0xc0 /* PSP: CollisionBox (0xb4) rounded up to 16 */, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
      mem->vtbl = g_collisionBoxVtbl;
      mem->invValid = 0;
      mem->type = 6;
      box = mem;
    }
    self->shape = box;
  }

  box = (CollisionBox *)self->shape;
  box->aabbMin.x = a[0];
  box->aabbMin.y = a[1];
  box->aabbMin.z = a[2];
  box->aabbMin.w = a[3];
  box->aabbMax.x = b[0];
  box->aabbMax.y = b[1];
  box->aabbMax.z = b[2];
  box->aabbMax.w = b[3];
  box->invValid = 0;
  box->transform = g_gfxIdentityMatrix;

  box = (CollisionBox *)self->shape;
  if (box->invValid == 0) {
    m = &box->transform;
    inv = &box->invTransform;
    inv->x.x = m->x.x;
    inv->x.y = m->y.x;
    inv->x.z = m->z.x;
    inv->x.w = 0.0f;
    inv->y.x = m->x.y;
    inv->y.y = m->y.y;
    inv->y.z = m->z.y;
    inv->y.w = 0.0f;
    inv->z.x = m->x.z;
    inv->z.y = m->y.z;
    inv->z.z = m->z.z;
    inv->z.w = 0.0f;
    inv->w.x = -(m->x.x * m->w.x + m->x.y * m->w.y + m->x.z * m->w.z);
    inv->w.y = -(m->y.x * m->w.x + m->y.y * m->w.y + m->y.z * m->w.z);
    inv->w.z = -(m->z.x * m->w.x + m->z.y * m->w.y + m->z.z * m->w.z);
    inv->w.w = m->w.w;
    box->invValid = 1;
  }
}
