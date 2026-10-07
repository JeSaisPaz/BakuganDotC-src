// bdc 0x088de388 ActorProbeGround
#include "bdc.h"

/* Unless disabled (`noGroundProbe`), casts the ground ray from the body-collider sphere centre
   along `g_vecDown` against the colliders of layer mask 0x3fbf2700 (`CollisionRaycast`,
   `g_collisionRayBlock`); the ray's `invDir` is 1/dir per xyz lane (0 where dir is 0), w 0.
   No hit: copies the ray origin's X and Z into `groundPoint` and sets flag bit 3 of `flags`
   (no floor below). Hit: stores the hit point as `groundPoint`, eases `groundNormal` 50% toward
   the hit normal, renormalises its xyz (each lane clamped to [-1,1], 0 for a zero-length
   vector) and zeroes its w. Records the hit surface in `surfaceType`. */

void ActorProbeGround(Actor *self)
{
  CollisionSphere *sphere;
  float dx, dy, dz;
  float nx, ny, nz, nw;
  float len2, k;

  if (self->noGroundProbe != 0) {
    return;
  }
  sphere = (CollisionSphere *)((CollisionCollider *)self->bodyCollider)->shapeDesc;
  g_collisionRayDesc.origin.x = sphere->center[0];
  g_collisionRayDesc.origin.y = sphere->center[1];
  g_collisionRayDesc.origin.z = sphere->center[2];
  g_collisionRayDesc.origin.w = sphere->radiusSq;
  g_collisionRayDesc.dir.x = g_vecDown.x;
  g_collisionRayDesc.dir.y = g_vecDown.y;
  g_collisionRayDesc.dir.z = g_vecDown.z;
  g_collisionRayDesc.dir.w = g_vecDown.w;
  dx = g_collisionRayDesc.dir.x;
  dy = g_collisionRayDesc.dir.y;
  dz = g_collisionRayDesc.dir.z;
  g_collisionRayDesc.invDir.x = (dx == 0.0f) ? 0.0f : VfRcp(dx);
  g_collisionRayDesc.invDir.y = (dy == 0.0f) ? 0.0f : VfRcp(dy);
  g_collisionRayDesc.invDir.z = (dz == 0.0f) ? 0.0f : VfRcp(dz);
  g_collisionRayDesc.invDir.w = 0.0f;
  if (CollisionRaycast(0x3fbf2700, &g_collisionRayBlock, 0) != (void *)0) {
    self->groundPoint[0] = g_collisionHitResult.point.x;
    self->groundPoint[1] = g_collisionHitResult.point.y;
    self->groundPoint[2] = g_collisionHitResult.point.z;
    self->groundPoint[3] = g_collisionHitResult.point.w;
    nx = self->groundNormal[0];
    ny = self->groundNormal[1];
    nz = self->groundNormal[2];
    nw = self->groundNormal[3];
    nx = nx + (g_collisionHitResult.normal.x - nx) * 0.5f;
    ny = ny + (g_collisionHitResult.normal.y - ny) * 0.5f;
    nz = nz + (g_collisionHitResult.normal.z - nz) * 0.5f;
    nw = nw + (g_collisionHitResult.normal.w - nw) * 0.5f;
    self->groundNormal[0] = nx;
    self->groundNormal[1] = ny;
    self->groundNormal[2] = nz;
    self->groundNormal[3] = nw;
    len2 = nx * nx + ny * ny + nz * nz;
    k = VfRsq(len2);
    if (len2 == 0.0f) {
      k = 0.0f;
    }
    self->groundNormal[0] = VfSat1(nx * k);
    self->groundNormal[1] = VfSat1(ny * k);
    self->groundNormal[2] = VfSat1(nz * k);
    self->groundNormal[3] = 0.0f;
    self->surfaceType = g_collisionHitInfo.surface;
  } else {
    self->groundPoint[0] = g_collisionRayDesc.origin.x;
    self->groundPoint[2] = g_collisionRayDesc.origin.z;
    self->flags = self->flags | 8;
  }
}
