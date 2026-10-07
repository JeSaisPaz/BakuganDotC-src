// bdc 0x088d78ac GameGimmickJetDoorCtor
#include "bdc.h"

/* Constructor of the Marucho-jet cabin door gimmick (kind 0x11 `EW_GMKOBJ_MARUCHO_JET_DOOR`,
   menu_cabin_door01.gmo; 0x1b0 bytes, spawned by `GameFieldLoadObjects`): `GameGimmickCtor`,
   vtables `g_gameGimmickJetDoorVtbl` / `g_gameGimmickJetDoorVtbl2`. Scales the record vector
   `+0x28` (20.12) by 20 (stored only to a dead stack temporary), moves the model position into
   `triggerPos` and zeroes `pos`, clears `lighting`, sets motion frame 0, calls vtable slot 6 with
   0.0f and turns motion looping off. Rebuilds the root matrix as the Y rotation by `rot[1]` scaled
   by (1, 1, 1) with translation `pos` (now 0) and w = 1, then clears `open`/`opening`, sets
   `lightScale = 1`, `lightStep = lightFrame = 0` and loads `lightFrom/To/Start/End` from
   `g_gameGimmickJetDoorBlinkTable``[0]`. Returns `obj`. */

CoreObject *GameGimmickJetDoorCtor(GameGimmickJetDoor *obj, s32 kind, void *record, u16 typeId, u8 flag)
{
  GameGimmickRecord *rec;
  const VtblEntry *entry;
  const GameGimmickJetDoorBlinkStep *step;
  float *m;
  float scaled[4];
  float tmp[4];
  float scale[4];
  float angle;
  float c;
  float s;
  int i;

  GameGimmickCtor(&obj->base, kind, record, typeId, flag);
  obj->base.base.base.vtable = g_gameGimmickJetDoorVtbl;
  obj->base.vtbl2 = g_gameGimmickJetDoorVtbl2;

  rec = (GameGimmickRecord *)obj->base.record;
  scaled[0] = (float)rec->jetDoorVec[0] * 0.000244140625f;
  scaled[1] = (float)rec->jetDoorVec[1] * 0.000244140625f;
  scaled[2] = (float)rec->jetDoorVec[2] * 0.000244140625f;
  scaled[3] = 0.0f;
  /* dead stack temporary: xyz * 20, w = bank S713 (0) */
  tmp[0] = scaled[0] * 20.0f;
  tmp[1] = scaled[1] * 20.0f;
  tmp[2] = scaled[2] * 20.0f;
  tmp[3] = 0.0f;
  (void)tmp;

  obj->triggerPos.x = obj->base.base.pos[0];
  obj->triggerPos.y = obj->base.base.pos[1];
  obj->triggerPos.z = obj->base.base.pos[2];
  obj->triggerPos.w = obj->base.base.pos[3];
  obj->base.base.pos[0] = 0.0f;
  obj->base.base.pos[1] = 0.0f;
  obj->base.base.pos[2] = 0.0f;
  obj->base.base.pos[3] = 0.0f;
  obj->base.base.lighting = 0;

  GfxModelSwapMotionFrame(&obj->base.base, 0.0f);
  entry = &((const VtblEntry *)obj->base.base.base.vtable)[6];
  ((float (*)(void *, float))entry->fn)((u8 *)obj + entry->delta, 0.0f);
  GfxModelSetMotionLoop(&obj->base.base, 0);

  /* Y rotation by rot[1] (vrot of rot[1] * S703 quarter turns) */
  m = obj->base.base.data->rootMatrix;
  angle = obj->base.base.rot[1];
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

  /* scale the three axis columns by (1, 1, 1) */
  m = obj->base.base.data->rootMatrix;
  scale[2] = 1.0f;
  scale[1] = 1.0f;
  scale[0] = 1.0f;
  scale[3] = 0.0f;
  for (i = 0; i < 4; i++) {
    m[i] = m[i] * scale[0];
    m[4 + i] = m[4 + i] * scale[1];
    m[8 + i] = m[8 + i] * scale[2];
  }

  m = obj->base.base.data->rootMatrix;
  m[12] = obj->base.base.pos[0];
  m[13] = obj->base.base.pos[1];
  m[14] = obj->base.base.pos[2];
  m[15] = obj->base.base.pos[3];
  obj->base.base.data->rootMatrix[15] = 1.0f;

  obj->open = 0;
  obj->opening = 0;
  obj->lightScale = 1.0f;
  obj->lightStep = 0;
  obj->lightFrame = 0;
  step = &g_gameGimmickJetDoorBlinkTable[obj->lightStep];
  obj->lightFrom = (u8)step->from;
  obj->lightTo = (u8)step->to;
  obj->lightStart = (u8)step->start;
  obj->lightEnd = (u8)step->end;
  return &obj->base.base.base;
}
