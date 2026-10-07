// bdc 0x088d6a88 GameGimmickIrSensorCtor
#include "bdc.h"

/* Constructor of the IR sensor beam gimmick (`EW_GMKOBJ_IRSensor`, gfx_102s.gmo):
   `GameGimmickCtor`, vtables `g_gameGimmickIrSensorVtbl` / `g_gameGimmickIrSensorVtbl2`,
   inline segment shape (`shapeType = 2`, `g_collisionSegmentVtbl`), `flicker = 0`. The beam
   runs from the model position to the record's second point `beamEnd` (20.12, x20): the model's
   root-matrix X axis is set to distance / 26 (Y/Z diagonal 1), the model is moved to the segment
   midpoint, its root matrix multiplied by a rotation about Y by -atan2f(dz, dx) (`R * Mᵀ` in the
   column view, see below) and its translation set to the new position (w = 1). `attached`,
   `state`, `step` and `shapeOwner` are cleared; `beamStart` = the original position 10 units up,
   `beamDir` = end point (y raised the same, w = 0) minus `beamStart`, then the shape's recalc
   virtual (slot 9, `CollisionShapeRecalcNop`) runs. `lighting = 1`, `color` = (0, 0, 0, 0),
   `ambient` = (1, 1, 1, 1), `mode` = record `variant & 0xf`; mode 1 allocates the 8-byte
   `GameGimmickIrSensorBeamTimer` from the low heap (on = `beamStartOn & 1`, count / onTime /
   offTime = 5 x `beamStartCount` / `beamOnTime` / `beamOffTime`), otherwise `beamTimer = NULL`.
   Returns `obj`. */

CoreObject *GameGimmickIrSensorCtor(GameGimmickIrSensor *obj, s32 kind, void *record, u16 typeId, u8 flag)
{
  GameGimmickRecord *rec;
  GameGimmickIrSensorBeamTimer *timer;
  const VtblEntry *e;
  GmoModel *model;
  bool fromLow;
  float len;
  float yaw;
  float c;
  float s;
  int i;
  float m[16];
  float start[4];
  float endRaw[3];
  float end[4];
  float diff[4];
  float p[4];

  GameGimmickCtor(&obj->base, kind, record, typeId, flag);
  obj->base.base.base.vtable = g_gameGimmickIrSensorVtbl;
  obj->base.vtbl2 = g_gameGimmickIrSensorVtbl2;
  obj->shapeVtbl = g_collisionSegmentVtbl;
  obj->shapeType = 2;
  obj->flicker = 0;

  start[0] = obj->base.base.pos[0];
  start[1] = obj->base.base.pos[1];
  start[2] = obj->base.base.pos[2];
  start[3] = obj->base.base.pos[3];
  rec = (GameGimmickRecord *)obj->base.record;
  endRaw[0] = (float)rec->beamEnd[0] * (1.0f / 4096.0f);
  endRaw[1] = (float)rec->beamEnd[1] * (1.0f / 4096.0f);
  endRaw[2] = (float)rec->beamEnd[2] * (1.0f / 4096.0f);
  /* end = endRaw * 20 (w from the bank's S713 = 0), diff = end - start (w = end.w) */
  end[0] = endRaw[0] * 20.0f;
  end[1] = endRaw[1] * 20.0f;
  end[2] = endRaw[2] * 20.0f;
  end[3] = 0.0f;
  diff[0] = end[0] - start[0];
  diff[1] = end[1] - start[1];
  diff[2] = end[2] - start[2];
  diff[3] = end[3];
  len = __builtin_sqrtf(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
  model = obj->base.base.data;
  model->rootMatrix[0] = len * 0.03846154f;
  model->rootMatrix[5] = 1.0f;
  model->rootMatrix[10] = 1.0f;
  yaw = -atan2f(diff[2], diff[0]);

  /* diff = half of it (w = S713 = 0), pos.xyz += diff.xyz */
  diff[0] = diff[0] * 0.5f;
  diff[1] = diff[1] * 0.5f;
  diff[2] = diff[2] * 0.5f;
  diff[3] = 0.0f;
  obj->base.base.pos[0] = obj->base.base.pos[0] + diff[0];
  obj->base.base.pos[1] = obj->base.base.pos[1] + diff[1];
  obj->base.base.pos[2] = obj->base.base.pos[2] + diff[2];

  /* vmmul.q E200, E100, E000 with M100 = rootMatrix (row i of the float[16] = column i) and
     M000 = columns (c, 0, -s, 0), (0, 1, 0, 0), (s, 0, c, 0), (0, 0, 0, 1): the result is
     R * M100ᵀ, so new[4i + r] = Σ_k R[r][k] * old[4k + i]. */
  model = obj->base.base.data;
  c = __builtin_cosf(yaw);
  s = __builtin_sinf(yaw);
  for (i = 0; i < 16; i++) {
    m[i] = model->rootMatrix[i];
  }
  for (i = 0; i < 4; i++) {
    model->rootMatrix[4 * i + 0] = c * m[i] + s * m[8 + i];
    model->rootMatrix[4 * i + 1] = m[4 + i];
    model->rootMatrix[4 * i + 2] = -s * m[i] + c * m[8 + i];
    model->rootMatrix[4 * i + 3] = m[12 + i];
  }

  model = obj->base.base.data;
  model->rootMatrix[12] = obj->base.base.pos[0];
  model->rootMatrix[13] = obj->base.base.pos[1];
  model->rootMatrix[14] = obj->base.base.pos[2];
  model->rootMatrix[15] = obj->base.base.pos[3];
  obj->base.base.data->rootMatrix[15] = 1.0f;
  obj->base.attached = NULL;
  obj->base.state = 0;
  obj->step = 0;
  obj->shapeOwner = NULL;

  /* beamStart = start 10 up; beamDir = (end with the same y) - beamStart, w = end.w */
  p[0] = start[0];
  p[1] = start[1] + 10.0f;
  p[2] = start[2];
  p[3] = start[3];
  end[1] = p[1];
  obj->beamStart.x = p[0];
  obj->beamStart.y = p[1];
  obj->beamStart.z = p[2];
  obj->beamStart.w = p[3];
  obj->beamDir.x = end[0] - p[0];
  obj->beamDir.y = end[1] - p[1];
  obj->beamDir.z = end[2] - p[2];
  obj->beamDir.w = end[3];
  e = &obj->shapeVtbl[9];
  ((void (*)(void *))e->fn)((u8 *)&obj->shapeType + e->delta);

  obj->base.base.lighting = 1;
  obj->base.base.color[0] = 0.0f;
  obj->base.base.color[1] = 0.0f;
  obj->base.base.color[2] = 0.0f;
  obj->base.base.color[3] = 0.0f;
  obj->base.base.ambient[0] = 1.0f;
  obj->base.base.ambient[1] = 1.0f;
  obj->base.base.ambient[2] = 1.0f;
  obj->base.base.ambient[3] = 1.0f;
  obj->mode = ((GameGimmickRecord *)obj->base.record)->variant & 0xf;
  obj->beamTimer = NULL;
  if (obj->mode == 1) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    timer = MemAlloc(sizeof(GameGimmickIrSensorBeamTimer), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    rec = (GameGimmickRecord *)obj->base.record;
    obj->beamTimer = (u8 *)timer;
    timer->on = rec->beamStartOn & 1;
    ((GameGimmickIrSensorBeamTimer *)obj->beamTimer)->count =
        ((GameGimmickRecord *)obj->base.record)->beamStartCount * 5;
    ((GameGimmickIrSensorBeamTimer *)obj->beamTimer)->onTime =
        ((GameGimmickRecord *)obj->base.record)->beamOnTime * 5;
    ((GameGimmickIrSensorBeamTimer *)obj->beamTimer)->offTime =
        ((GameGimmickRecord *)obj->base.record)->beamOffTime * 5;
  }
  return &obj->base.base.base;
}
