// bdc 0x088ab34c ActorStageObjBaseCtorByName
#include "bdc.h"

/* Variant of `ActorStageObjBaseCtor` for objects that are not in the kind table: builds the model
   from `gmoName` (`GfxModelCtor`, 0x200), installs the base vtable `0x08af2904`, runs
   `ActorStageObjInitFields`, clears `dead`/`removeRequest`, sets the model matrix to a Y rotation
   by `pos[3]` degrees with translation `pos`, stores the heading `pi/2 - pos[3]` (radians) at
   `rot[1]` (`+0x34`), copies the translation to `pos`, lighting on, ambient {0.3, 0.3, 0.3, 1.0},
   `fade` 1.0, keeps a copy of `pos` in `gaugePos` (`+0x2a0`) and appends the object to the
   stage-object chain `g_actorStageObjList` (`CoreObjectListAppend`). Returns `self`. */

ActorStageObjBase *ActorStageObjBaseCtorByName(ActorStageObjBase *self, const char *gmoName, const float *pos)
{
  float *m;
  float angle;
  float c;
  float s;
  float ambient;

  GfxModelCtor(&self->base, gmoName, 0x200);
  self->base.base.vtable = &g_actorStageObjBaseVtbl;
  ActorStageObjInitFields(self);
  self->dead = 0;
  self->removeRequest = 0;
  m = self->base.data->rootMatrix;
  angle = pos[3] * 0.017453292f;
  c = __builtin_cosf(angle);
  s = __builtin_sinf(angle);
  m[0] = c;
  m[1] = 0.0f;
  m[2] = -s;
  m[3] = 0.0f;
  m[4] = 0.0f;
  m[5] = 1.0f;
  m[6] = 0.0f;
  m[7] = 0.0f;
  m[8] = s;
  m[9] = 0.0f;
  m[10] = c;
  m[11] = 0.0f;
  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = 1.0f;
  m = self->base.data->rootMatrix;
  m[12] = pos[0];
  m[13] = pos[1];
  m[14] = pos[2];
  m[15] = pos[3];
  self->base.rot[1] = pos[3] * -0.017453292f + 1.5707964f;
  m = self->base.data->rootMatrix;
  self->base.pos[0] = m[12];
  self->base.pos[1] = m[13];
  self->base.pos[2] = m[14];
  self->base.pos[3] = m[15];
  self->base.lighting = 1;
  ambient = 0.3f;
  self->base.ambient[0] = ambient;
  self->base.ambient[1] = ambient;
  self->base.ambient[2] = ambient;
  self->base.ambient[3] = 1.0f;
  self->fade = 1.0f;
  self->gaugePos[0] = pos[0];
  self->gaugePos[1] = pos[1];
  self->gaugePos[2] = pos[2];
  self->gaugePos[3] = pos[3];
  CoreObjectListAppend((CoreObject *)self, g_actorStageObjList);
  return self;
}
