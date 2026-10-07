// bdc 0x088de648 ActorUpdate
#include "bdc.h"

/* Per-frame update of the base actor class (vtable slot 7, `+0x3c`; the player edit-man class
   overrides it with `ActorPlayerUpdate`): zeroes the velocity (the VFPU bank zero vector C720), runs the distance cull/fade slot 5 (`ActorUpdateDistanceFade`, 23..1000 units,
   alpha `ambient[3]`) unless this is the player, reads the input actions into `motion`
   (`BtlInputReadActions`; while the `motionOverride` countdown runs `motion` is 0, and
   `motionOverrideTimer` when it reaches 0), masks `flags`, calls the behaviour slot `+0xfc` and then
   either only the minimal update (culled: `ActorApplyGravity` when moving, slot `+0x54`, shadow
   disabled and `BtlShadowUpdate`) or the full one: `ActorApplyGravity` when moving,
   `GfxModelUpdateMotion`/`GfxModelApplyMotion`, slot `+0x54`, `ActorSyncColliders`,
   `ActorProbeGround`, shadow enable (except models 0x54/0x55) and `BtlShadowUpdate`,
   `ActorUpdateFootsteps` and, unless `flags` bit 0, easing `tiltQuat` 20% toward `g_vecUp` and
   renormalising its xyz (clamped to [-1, 1], factor 0 for a zero vector) with w set to 0 (bank S713). */

void ActorUpdate(Actor *self)
{
  const VtblEntry *vt;
  const ActorNpcPlacement *placement;
  s32 culled;
  s32 moving;
  s32 motion;
  s32 left;
  float speedSq;
  float *q;
  float x, y, z, w;
  float lenSq;
  float inv;

  /* velocity = C720, the bank zero vector */
  self->base.velocity[0] = 0.0f;
  self->base.velocity[1] = 0.0f;
  self->base.velocity[2] = 0.0f;
  self->base.velocity[3] = 0.0f;
  culled = 0;
  if (self->isPlayer == 0) {
    vt = &((const VtblEntry *)self->base.base.vtable)[5];
    if (((s32 (*)(void *, float *, float *, float, float))vt->fn)(
            (u8 *)self + vt->delta, &self->base.data->rootMatrix[12], &self->base.ambient[3],
            23.0f, 1000.0f) != 0) {
      culled = 1;
    }
  }
  self->motion = BtlInputReadActions((BtlInput *)self->input);
  if (self->motionOverride != 0) {
    motion = 0;
    left = self->motionOverride - 1;
    self->motionOverride = left;
    if (left == 0) {
      motion = self->motionOverrideTimer;
    }
    self->motion = motion;
  }
  self->flags &= 0xce1ffff8;
  vt = &((const VtblEntry *)self->base.base.vtable)[31];
  ((void (*)(void *))vt->fn)((u8 *)self + vt->delta);

  /* |velocity|^2 (xyz), read after the behaviour slot */
  speedSq = self->base.velocity[0] * self->base.velocity[0] +
            self->base.velocity[1] * self->base.velocity[1] +
            self->base.velocity[2] * self->base.velocity[2];
  moving = 0;
  if (!(speedSq <= 1e-05f)) {
    moving = 1;
  }
  if (self->placement != NULL) {
    placement = (const ActorNpcPlacement *)self->placement;
    if (placement->charCode == 7 || placement->charCode == 10) {
      culled = 0;
      moving = 1;
    }
  }

  if (culled != 0) {
    if (moving != 0) {
      ActorApplyGravity(self);
    }
    vt = &((const VtblEntry *)self->base.base.vtable)[10];
    ((void (*)(void *))vt->fn)((u8 *)self + vt->delta);
    ((BtlShadow *)self->shadow)->enabled = 0;
    BtlShadowUpdate(self->shadow);
    return;
  }

  self->flags &= 0x7ffdfdef;
  if (moving != 0) {
    ActorApplyGravity(self);
  }
  GfxModelUpdateMotion(&self->base);
  GfxModelApplyMotion(&self->base);
  vt = &((const VtblEntry *)self->base.base.vtable)[10];
  ((void (*)(void *))vt->fn)((u8 *)self + vt->delta);
  ActorSyncColliders(self);
  ActorProbeGround(self);
  if (self->base.base.unk08 != 0x54 && self->base.base.unk08 != 0x55) {
    ((BtlShadow *)self->shadow)->enabled = 1;
  }
  BtlShadowUpdate(self->shadow);
  ActorUpdateFootsteps(self);
  if ((self->flags & 1) == 0) {
    /* tiltQuat += (g_vecUp - tiltQuat) * 0.2; then xyz normalised and clamped to [-1, 1] (factor
       0 for a zero length); w is the bank S713 = 0 */
    q = self->tiltQuat;
    x = q[0] + (g_vecUp.x - q[0]) * 0.2f;
    y = q[1] + (g_vecUp.y - q[1]) * 0.2f;
    z = q[2] + (g_vecUp.z - q[2]) * 0.2f;
    w = q[3] + (g_vecUp.w - q[3]) * 0.2f;
    q[0] = x;
    q[1] = y;
    q[2] = z;
    q[3] = w;
    lenSq = x * x + y * y + z * z;
    inv = VfRsq(lenSq);
    if (lenSq == 0.0f) {
      inv = 0.0f;
    }
    q[0] = VfSat1(x * inv);
    q[1] = VfSat1(y * inv);
    q[2] = VfSat1(z * inv);
    q[3] = 0.0f;
  }
}
