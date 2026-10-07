// bdc 0x08863b8c BtlBakuganProbeGround
#include "bdc.h"

/* Once-per-frame ground probe of a battle Bakugan (does nothing when `groundProbed` is already
   set, otherwise sets it). A player unit without flag 0x1000 in `flags` first sweeps the static
   swept sphere `g_collisionSweptSphereDesc` (radius `stats->bodyRadius * 0.7`, from the
   position 3000 units along `g_vecDown`); on a hit the ground point is the position with the
   hit height, and on flat ground (not a closest-point hit, normal.y > 0.5, surface < 6) the
   ground normal is blended 70 % towards the hit normal. Without a sphere hit, and for every
   other unit, a ray `g_collisionRayDesc` is cast down from the position raised by `height`;
   on a hit the ground point is the hit point (other units also take the hit normal). Every
   hit stores the floor material and clears the airborne flag 8 of `stateFlags`; with no hit
   the ground point takes the unit's x/z and flag 8 is set. All casts use `CollisionRaycast`
   with mask `0x3fbf2700`. Outside states 3..6 and without the `0x40000000`/`0x1000000` state
   flags, `groundHeight` latches the current height.
   The sweep direction's w and the ray's inverse direction w (and any zero-direction lane) are
   0 (VFPU bank constant S713). */

void BtlBakuganProbeGround(BtlBakugan *self)
{
  float *pos;
  float radius;
  float k;
  void *hit;
  s32 found;

  if (self->groundProbed != 0) {
    return;
  }
  found = 0;
  self->groundProbed = 1;
  pos = self->base.pos;
  if ((self->flags & 0x1000) == 0 && self->isPlayer != 0) {
    radius = self->combat.stats->bodyRadius * 0.7f;
    g_collisionSweptSphereDesc.radius = radius;
    g_collisionSweptSphereDesc.start.x = pos[0];
    g_collisionSweptSphereDesc.start.y = pos[1];
    g_collisionSweptSphereDesc.start.z = pos[2];
    g_collisionSweptSphereDesc.start.w = pos[3];
    g_collisionSweptSphereDesc.dir.x = g_vecDown.x * 3000.0f;
    g_collisionSweptSphereDesc.dir.y = g_vecDown.y * 3000.0f;
    g_collisionSweptSphereDesc.dir.z = g_vecDown.z * 3000.0f;
    g_collisionSweptSphereDesc.dir.w = 0.0f;
    g_collisionSweptSphereDesc.start.w =
        g_collisionSweptSphereDesc.radius * g_collisionSweptSphereDesc.radius;
    g_collisionSweptSphereDesc.dir.w = __builtin_sqrtf(
        g_collisionSweptSphereDesc.dir.x * g_collisionSweptSphereDesc.dir.x +
        g_collisionSweptSphereDesc.dir.y * g_collisionSweptSphereDesc.dir.y +
        g_collisionSweptSphereDesc.dir.z * g_collisionSweptSphereDesc.dir.z);
    hit = CollisionRaycast(0x3fbf2700, g_collisionSweptSphereDesc.shapeBlock, 0);
    if (hit != (void *)0) {
      self->groundPoint[0] = pos[0];
      self->groundPoint[1] = pos[1];
      self->groundPoint[2] = pos[2];
      self->groundPoint[3] = pos[3];
      self->groundY = g_collisionHitResult.point.y;
      if (g_collisionHitInfo.fromClosestPoint == 0 &&
          !(g_collisionHitResult.normal.y <= 0.5f) && g_collisionHitInfo.surface < 6) {
        /* groundNormal += (hit normal - groundNormal) * 0.7f */
        k = 0.7f;
        self->groundNormal[0] =
            self->groundNormal[0] + (g_collisionHitResult.normal.x - self->groundNormal[0]) * k;
        self->groundNormal[1] =
            self->groundNormal[1] + (g_collisionHitResult.normal.y - self->groundNormal[1]) * k;
        self->groundNormal[2] =
            self->groundNormal[2] + (g_collisionHitResult.normal.z - self->groundNormal[2]) * k;
        self->groundNormal[3] =
            self->groundNormal[3] + (g_collisionHitResult.normal.w - self->groundNormal[3]) * k;
      }
      self->floorMaterial = g_collisionHitInfo.surface;
      self->stateFlags &= ~8u;
      found = 1;
    } else {
      g_collisionRayDesc.origin.x = pos[0];
      g_collisionRayDesc.origin.y = pos[1];
      g_collisionRayDesc.origin.z = pos[2];
      g_collisionRayDesc.origin.w = pos[3];
      g_collisionRayDesc.origin.y = g_collisionRayDesc.origin.y + self->height;
      g_collisionRayDesc.dir.x = g_vecDown.x;
      g_collisionRayDesc.dir.y = g_vecDown.y;
      g_collisionRayDesc.dir.z = g_vecDown.z;
      g_collisionRayDesc.dir.w = g_vecDown.w;
      /* invDir.xyz = 1/dir.xyz, or 0 where dir.i == 0; invDir.w = 0. */
      g_collisionRayDesc.invDir.x =
          (g_collisionRayDesc.dir.x == 0.0f) ? 0.0f : 1.0f / g_collisionRayDesc.dir.x;
      g_collisionRayDesc.invDir.y =
          (g_collisionRayDesc.dir.y == 0.0f) ? 0.0f : 1.0f / g_collisionRayDesc.dir.y;
      g_collisionRayDesc.invDir.z =
          (g_collisionRayDesc.dir.z == 0.0f) ? 0.0f : 1.0f / g_collisionRayDesc.dir.z;
      g_collisionRayDesc.invDir.w = 0.0f;
      hit = CollisionRaycast(0x3fbf2700, &g_collisionRayBlock, 0);
      if (hit != (void *)0) {
        self->groundPoint[0] = g_collisionHitResult.point.x;
        self->groundPoint[1] = g_collisionHitResult.point.y;
        self->groundPoint[2] = g_collisionHitResult.point.z;
        self->groundPoint[3] = g_collisionHitResult.point.w;
        self->floorMaterial = g_collisionHitInfo.surface;
        self->stateFlags &= ~8u;
        found = 1;
      }
    }
  } else {
    g_collisionRayDesc.origin.x = pos[0];
    g_collisionRayDesc.origin.y = pos[1];
    g_collisionRayDesc.origin.z = pos[2];
    g_collisionRayDesc.origin.w = pos[3];
    g_collisionRayDesc.origin.y = g_collisionRayDesc.origin.y + self->height;
    g_collisionRayDesc.dir.x = g_vecDown.x;
    g_collisionRayDesc.dir.y = g_vecDown.y;
    g_collisionRayDesc.dir.z = g_vecDown.z;
    g_collisionRayDesc.dir.w = g_vecDown.w;
    /* invDir.xyz = 1/dir.xyz, or 0 where dir.i == 0; invDir.w = 0. */
    g_collisionRayDesc.invDir.x =
        (g_collisionRayDesc.dir.x == 0.0f) ? 0.0f : 1.0f / g_collisionRayDesc.dir.x;
    g_collisionRayDesc.invDir.y =
        (g_collisionRayDesc.dir.y == 0.0f) ? 0.0f : 1.0f / g_collisionRayDesc.dir.y;
    g_collisionRayDesc.invDir.z =
        (g_collisionRayDesc.dir.z == 0.0f) ? 0.0f : 1.0f / g_collisionRayDesc.dir.z;
    g_collisionRayDesc.invDir.w = 0.0f;
    hit = CollisionRaycast(0x3fbf2700, &g_collisionRayBlock, 0);
    if (hit != (void *)0) {
      self->groundPoint[0] = g_collisionHitResult.point.x;
      self->groundPoint[1] = g_collisionHitResult.point.y;
      self->groundPoint[2] = g_collisionHitResult.point.z;
      self->groundPoint[3] = g_collisionHitResult.point.w;
      self->groundNormal[0] = g_collisionHitResult.normal.x;
      self->groundNormal[1] = g_collisionHitResult.normal.y;
      self->groundNormal[2] = g_collisionHitResult.normal.z;
      self->groundNormal[3] = g_collisionHitResult.normal.w;
      self->floorMaterial = g_collisionHitInfo.surface;
      self->stateFlags &= ~8u;
      found = 1;
    }
  }
  if (!found) {
    self->groundPoint[0] = self->base.pos[0];
    self->groundPoint[2] = self->base.pos[2];
    self->stateFlags |= 8;
  }
  if (self->state > 2 && self->state < 7) {
    return;
  }
  if ((self->stateFlags & 0x40000000) == 0 && (self->stateFlags & 0x1000000) == 0 &&
      (self->stateFlags & 0x40000000) == 0) {
    self->groundHeight = self->base.pos[1];
  }
}
