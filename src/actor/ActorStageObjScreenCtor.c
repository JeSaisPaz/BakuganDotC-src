// bdc 0x088b05ec ActorStageObjScreenCtor
#include "bdc.h"

/* Constructor of the screen stage objects (category 10: kinds 0xb6/0xb7 `SCREEN_00/01`,
   `ffx_120m.gmo`, 0x340 bytes): `ActorStageObjBaseCtorByName` with the kind's model from
   `g_actorStageObjModelTable`, vtable `g_actorStageObjScreenVtbl`, radius = 0.7 * bounds
   diagonal, halfHeight = half the bounds height, lighting off, fade 0.999, hides the `noise`
   node, installs the material callback `ActorStageObjScreenMaterialAdditive` and sets `shown`.
   Returns self. */

void *ActorStageObjScreenCtor(ActorStageObjScreen *self, int kind, float *pos)
{
  float p[4];
  float *max;
  float *min;
  float dx, dy, dz;
  float diag;
  float top;
  GmoNode *node;

  p[0] = pos[0];
  p[1] = pos[1];
  p[2] = pos[2];
  p[3] = pos[3];
  ActorStageObjBaseCtorByName(&self->base, g_actorStageObjModelTable[kind * 3], p);
  self->base.base.base.vtable = g_actorStageObjScreenVtbl;
  self->base.step = 0;
  self->unused328 = 0;
  self->state = 0;

  max = ActorStageObjGetBounds(&self->base) + 4;
  min = ActorStageObjGetBounds(&self->base);
  dx = max[0] - min[0];
  dy = max[1] - min[1];
  dz = max[2] - min[2];
  diag = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
  self->radius = diag * 0.7f;

  top = ActorStageObjGetBounds(&self->base)[5];
  self->halfHeight = (top - ActorStageObjGetBounds(&self->base)[1]) * 0.5f;

  self->base.base.lighting = 0;
  self->base.fade = 0.999f;
  node = (GmoNode *)GfxModelFindNode(&self->base.base, "noise");
  if (node != NULL) {
    node->visible = 0;
  }
  GfxModelForEachMaterial(&self->base.base, (void *)ActorStageObjScreenMaterialAdditive, NULL);
  self->shown = 1;
  return self;
}
