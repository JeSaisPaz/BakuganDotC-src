// bdc 0x088aab34 ActorStageObjPlace
#include "bdc.h"

/* Places a stage object at `pos` (Called by `ActorStageObjBaseInit`): ambient 0.3 (alpha 1);
   unless it is a landmark (`ActorStageObjIsLandmark`) `pos[1]` is lowered by the bounds' min Y
   (`ActorStageObjGetBounds`) and, for kinds 0x99/0x9a, by another 60. Converts the heading
   `pos[3]` from degrees to radians (written back to `pos[3]` and `rot[1]`), writes the
   Y-rotation × scale matrix into the model's root matrix and `pos` as its translation row, copies
   that row to `base.pos`, sets `lighting = 1`, `flags1d0 = 0x10`, `step = 0`, `hitBy = NULL` and
   `baseY = pos.y`. */

void ActorStageObjPlace(ActorStageObjBase *self, float *pos)
{
  float heading;
  float y;
  float *bounds;
  float *m;
  float c;
  float s;
  s32 kind;

  self->base.ambient[0] = 0.3f;
  self->base.ambient[1] = 0.3f;
  self->base.ambient[2] = 0.3f;
  self->base.ambient[3] = 1.0f;
  if (ActorStageObjIsLandmark(self) == 0) {
    y = pos[1];
    bounds = ActorStageObjGetBounds(self);
    pos[1] = y + -bounds[1];
    kind = self->kind;
    if (kind >= 0x99 && kind < 0x9b) {
      pos[1] = pos[1] - 60.0f;
    }
  }
  heading = pos[3] * 0.017453292f;
  pos[3] = heading;
  self->base.rot[1] = heading;
  /* Rotation about Y (vrot of heading * 2/π in quarter turns) times diag(scale.x, scale.y,
     scale.z, 1), fields as columns. */
  m = self->base.data->rootMatrix;
  c = __builtin_cosf(heading);
  s = __builtin_sinf(heading);
  m[0] = c * self->base.scale[0];
  m[1] = 0.0f;
  m[2] = -s * self->base.scale[0];
  m[3] = 0.0f;
  m[4] = 0.0f;
  m[5] = self->base.scale[1];
  m[6] = 0.0f;
  m[7] = 0.0f;
  m[8] = s * self->base.scale[2];
  m[9] = 0.0f;
  m[10] = c * self->base.scale[2];
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
  self->base.lighting = 1;
  m = self->base.data->rootMatrix;
  self->base.pos[0] = m[12];
  self->base.pos[1] = m[13];
  self->base.pos[2] = m[14];
  self->base.pos[3] = m[15];
  self->flags1d0 = 0x10;
  self->step = 0;
  self->hitBy = NULL;
  self->baseY = self->base.pos[1];
}
