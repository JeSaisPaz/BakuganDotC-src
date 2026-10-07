// bdc 0x08855800 ActorCrystalUpdateMatrices
#include "bdc.h"

/* Rebuilds the crystal's model matrix (`data->rootMatrix`): a Y rotation by pi/2 - heading
   (`rot[1]`, wrapped into (-pi, pi]) times the scale (`scale[0..2]`), translation = position with
   w = 1. If a stand exists, writes the same rotation (no scale) into the stand's model matrix,
   moves the stand to the crystal's `homePos` unless `BtlIsScoreMode(1)`, copies the stand position
   into its matrix translation, sets the stand's alpha (`ambient[3]`) to the crystal's `fade` and
   makes the stand collidable when `fade <= 0` (pass-through otherwise). */

void ActorCrystalUpdateMatrices(ActorCrystal *self)
{
  GfxModel *model;
  ActorCrystalStand *stand;
  float *m;
  float sx;
  float sy;
  float sz;
  float c;
  float s;
  float fade;
  float heading;

  model = &self->base.base;
  m = model->data->rootMatrix;
  heading = 1.5707964f - model->rot[1];
  if (!(heading <= 3.1415927f)) {
    heading = heading - 6.2831855f;
  } else if (heading <= -3.1415927f) {
    heading = heading + 6.2831855f;
  }
  /* rotation [cos,0,-sin,0] [0,1,0,0] [sin,0,cos,0] [0,0,0,1] times diag(scale, 1) */
  sx = model->scale[0];
  sy = model->scale[1];
  sz = model->scale[2];
  c = __builtin_cosf(heading);
  s = __builtin_sinf(heading);
  m[0] = c * sx;
  m[1] = 0.0f;
  m[2] = -s * sx;
  m[3] = 0.0f;
  m[4] = 0.0f;
  m[5] = sy;
  m[6] = 0.0f;
  m[7] = 0.0f;
  m[8] = s * sz;
  m[9] = 0.0f;
  m[10] = c * sz;
  m[11] = 0.0f;
  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = 1.0f;
  m = model->data->rootMatrix;
  m[12] = model->pos[0];
  m[13] = model->pos[1];
  m[14] = model->pos[2];
  m[15] = model->pos[3];
  model->data->rootMatrix[15] = 1.0f;
  if (self->stand == NULL) {
    return;
  }
  stand = (ActorCrystalStand *)self->stand;
  m = stand->base.data->rootMatrix;
  heading = 1.5707964f - model->rot[1];
  if (!(heading <= 3.1415927f)) {
    heading = heading - 6.2831855f;
  } else if (heading <= -3.1415927f) {
    heading = heading + 6.2831855f;
  }
  c = __builtin_cosf(heading);
  s = __builtin_sinf(heading);
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
  if (BtlIsScoreMode(1) == 0) {
    stand = (ActorCrystalStand *)self->stand;
    stand->base.pos[0] = self->homePos[0];
    stand->base.pos[1] = self->homePos[1];
    stand->base.pos[2] = self->homePos[2];
    stand->base.pos[3] = self->homePos[3];
  }
  stand = (ActorCrystalStand *)self->stand;
  m = stand->base.data->rootMatrix;
  m[12] = stand->base.pos[0];
  m[13] = stand->base.pos[1];
  m[14] = stand->base.pos[2];
  m[15] = stand->base.pos[3];
  fade = self->fade;
  ((ActorCrystalStand *)self->stand)->base.ambient[3] = fade;
  if (fade <= 0.0f) {
    ActorCrystalStandSetCollidable((ActorCrystalStand *)self->stand, 0);
  } else {
    ActorCrystalStandSetCollidable((ActorCrystalStand *)self->stand, 1);
  }
}
