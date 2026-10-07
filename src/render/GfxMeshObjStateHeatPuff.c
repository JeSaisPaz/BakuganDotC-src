// bdc 0x08826aa8 GfxMeshObjStateHeatPuff
#include "bdc.h"

/* State 1 state handler of the mesh object (`GfxMeshObjCtor`) (table `0x08ab9ebc`, run by
   `GfxMeshObjRunState`): a rising heat-haze puff.
   Sub-state `step`:
   0 setup, returns right after: draws `g_gfxDistortionPatchVerts` / `g_gfxDistortionIndices`
     as an indexed prim textured with the screen copy (`GfxGetFeedbackTexture`, a refraction
     effect), white at half alpha, `scaleA`/`scaleB` 0, random `radius` 22..32, a random sideways
     `velocity` (-1..1) along the camera heading `pi/2 - camera->yaw` wrapped to (-pi, pi],
     added once to `pos`, and `axis` = 0.03 × velocity (the per-frame velocity decrement);
   1 shown: radius +0.5, velocity x/z × 0.87, velocity -= axis, rising 0.08/frame, pos += velocity,
     pos -= camera `dir` × (0.2, 0, 0.2), alpha +0.091 and step 2 once alpha is not below 0.84;
   2 radius +0.5, the same damping, rising 0.08/frame only while velocity y < 1.5, pos += velocity,
     alpha -0.07, hidden and step 3 once alpha <= 0;
   3 deletes itself through the virtual destructor (nothing when `self` is NULL) and returns.
   Every other path (steps 1, 2, >= 4 and negative) re-fits the screen mapping with
   `GfxMeshObjFitScreenBillboard` at `radius`. Random numbers are `vrndf1.s` (1..2) minus 1. */

typedef struct GfxDtorEntry {
  short thisAdjust;
  short pad;
  void (*fn)(void *self, int flags);
} GfxDtorEntry;

typedef struct GfxDtorVtable {
  u32 hdr[2];
  GfxDtorEntry dtor;
} GfxDtorVtable;

/* velocity -= axis (xyz) */
static inline void HeatPuffDecelerate(GfxMeshObj *self)
{
  self->velocity[0] = self->velocity[0] - self->axis[0];
  self->velocity[1] = self->velocity[1] - self->axis[1];
  self->velocity[2] = self->velocity[2] - self->axis[2];
}

/* pos += velocity (xyz) */
static inline void HeatPuffMove(GfxMeshObj *self)
{
  self->pos[0] = self->pos[0] + self->velocity[0];
  self->pos[1] = self->pos[1] + self->velocity[1];
  self->pos[2] = self->pos[2] + self->velocity[2];
}

void GfxMeshObjStateHeatPuff(GfxMeshObj *self)
{
  int step;
  float heading;
  float side;
  float alpha;
  float c;
  float s;
  float pull[4];
  float tmp[3];
  const GfxDtorEntry *e;

  step = self->step;
  if (step < 2) {
    if (step < 0) {
      goto fit;
    }
    if (step <= 0) {
      self->vertices = g_gfxDistortionPatchVerts;
      self->blendMode = 1;
      self->drawKind = 1;
      self->primCmd = 0x1fd;
      self->texture = GfxGetFeedbackTexture();
      self->vertexType = 0x1200089a;
      self->indices = g_gfxDistortionIndices;
      self->emissive = 1;
      self->scaleA = 0.0f;
      self->scaleB = 0.0f;
      self->color[0] = 1.0f;
      self->color[1] = 1.0f;
      self->color[2] = 1.0f;
      self->color[3] = 0.5f;
      self->radius = (PlatformRandFloat12() - 1.0f) * 10.0f + 22.0f;
      self->step = self->step + 1;
      heading = 1.57079637f - g_gfxActiveCamera->yaw;
      if (!(heading <= 3.14159274f)) {
        heading = heading - 6.28318548f;
      } else if (heading <= -3.14159274f) {
        heading = heading + 6.28318548f;
      }
      side = (PlatformRandFloat12() - 1.0f) * 2.0f - 1.0f;
      /* vrot.q [C,0,S,0] of heading × 2/π, scaled (xyz) by side; w lane 0 */
      c = __builtin_cosf(heading);
      s = __builtin_sinf(heading);
      self->velocity[0] = c * side;
      self->velocity[1] = 0.0f * side;
      self->velocity[2] = s * side;
      self->velocity[3] = 0.0f;
      HeatPuffMove(self);
      /* axis = velocity × 0.03 (xyz); w is the bank's S713 (0) */
      self->axis[0] = self->velocity[0] * 0.0299999993f;
      self->axis[1] = self->velocity[1] * 0.0299999993f;
      self->axis[2] = self->velocity[2] * 0.0299999993f;
      self->axis[3] = 0.0f;
      return;
    }
    /* step 1 */
    self->visible = 1;
    self->radius = self->radius + 0.5f;
    self->velocity[0] = self->velocity[0] * 0.870000005f;
    self->velocity[2] = self->velocity[2] * 0.870000005f;
    HeatPuffDecelerate(self);
    self->velocity[1] = self->velocity[1] + 0.0799999982f;
    HeatPuffMove(self);
    pull[0] = 0.200000003f;
    pull[1] = 0.0f;
    pull[2] = 0.200000003f;
    pull[3] = 0.0f;
    /* pos -= camera dir × pull (xyz) */
    tmp[0] = g_gfxActiveCamera->dir[0] * pull[0];
    tmp[1] = g_gfxActiveCamera->dir[1] * pull[1];
    tmp[2] = g_gfxActiveCamera->dir[2] * pull[2];
    self->pos[0] = self->pos[0] - tmp[0];
    self->pos[1] = self->pos[1] - tmp[1];
    self->pos[2] = self->pos[2] - tmp[2];
    alpha = self->color[3] + 0.0909999982f;
    self->color[3] = alpha;
    if (!(alpha < 0.839999974f)) {
      self->step = self->step + 1;
    }
  } else if (step < 3) {
    /* step 2 */
    self->radius = self->radius + 0.5f;
    self->velocity[0] = self->velocity[0] * 0.870000005f;
    self->velocity[2] = self->velocity[2] * 0.870000005f;
    HeatPuffDecelerate(self);
    if (self->velocity[1] < 1.5f) {
      self->velocity[1] = self->velocity[1] + 0.0799999982f;
    }
    HeatPuffMove(self);
    alpha = self->color[3] - 0.0700000003f;
    self->color[3] = alpha;
    if (alpha <= 0.0f) {
      self->visible = 0;
      self->step = self->step + 1;
    }
  } else if (step < 4) {
    /* step 3 */
    if (self != NULL) {
      e = &((const GfxDtorVtable *)self->base.vtable)->dtor;
      e->fn((char *)self + e->thisAdjust, 3);
    }
    return;
  }
fit:
  GfxMeshObjFitScreenBillboard(self->radius, self);
}
