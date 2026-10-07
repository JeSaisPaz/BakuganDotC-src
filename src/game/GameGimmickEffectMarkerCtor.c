// bdc 0x088da5cc GameGimmickEffectMarkerCtor
#include "bdc.h"

/* Constructor of an effect-marker gimmick (400 bytes, spawned by `GameFieldLoadObjects` with kind
   0 for type ids 0x73, 0x74, 0x77): `GameGimmickCtor`, vtables `g_gameGimmickEffectMarkerVtbl` /
   `g_gameGimmickEffectMarkerVtbl2`, `state = 0`, stores the type id at `+0x188`, copies `pos` (all
   four lanes) into the model's root-matrix translation, clears `ambient[3]`, `attached`, `effect`,
   `effect2`, and spawns effects on `g_worldEffectMgr` at `pos`: id 0x73 gets effects 0xb (`effect`)
   and 0xc (`effect2`, alpha `color[3] = 0`), id 0x77 effect 0xd (`effect`); 0x74 spawns nothing. Each
   spawned effect's matrix is set to a Y rotation by the record heading (1/65536 turns, wrapped to
   (-pi, pi]) plus 3.14. Returns `obj`. */

CoreObject *GameGimmickEffectMarkerCtor(GameGimmickEffectMarker *obj, s32 kind, void *record, u16 typeId, u8 flag)
{
  GameGimmickRecord *rec;
  GfxEffect *effect;
  GmoModel *model;
  float heading;
  float angle;
  float c;
  float s;

  GameGimmickCtor(&obj->base, kind, record, typeId, flag);
  obj->base.base.base.vtable = g_gameGimmickEffectMarkerVtbl;
  obj->base.vtbl2 = g_gameGimmickEffectMarkerVtbl2;
  obj->base.state = 0;
  obj->typeId = typeId;
  model = obj->base.base.data;
  model->rootMatrix[12] = obj->base.base.pos[0];
  model->rootMatrix[13] = obj->base.base.pos[1];
  model->rootMatrix[14] = obj->base.base.pos[2];
  model->rootMatrix[15] = obj->base.base.pos[3];
  obj->base.base.ambient[3] = 0.0f;
  obj->base.attached = NULL;
  obj->effect = NULL;
  obj->effect2 = NULL;

  if (typeId == 0x73) {
    obj->effect = GfxEffectSpawn(g_worldEffectMgr, 0xb, obj->base.base.pos);
    obj->effect2 = GfxEffectSpawn(g_worldEffectMgr, 0xc, obj->base.base.pos);
    if (obj->effect2 != NULL) {
      rec = (GameGimmickRecord *)obj->base.record;
      heading = (float)(s32)rec->heading * 6.2831855f * 1.5259022e-05f;
      if (!(heading <= 3.1415927f)) {
        heading = heading - 6.2831855f;
      } else if (heading <= -3.1415927f) {
        heading = heading + 6.2831855f;
      }
      effect = (GfxEffect *)obj->effect2;
      /* vrot of (heading + 3.14) * S703 (2/pi): cos/sin of the angle in radians */
      angle = heading + 3.14f;
      c = __builtin_cosf(angle);
      s = __builtin_sinf(angle);
      effect->matrix[0] = c;
      effect->matrix[1] = 0.0f;
      effect->matrix[2] = -s;
      effect->matrix[3] = 0.0f;
      effect->matrix[4] = 0.0f;
      effect->matrix[5] = 1.0f;
      effect->matrix[6] = 0.0f;
      effect->matrix[7] = 0.0f;
      effect->matrix[8] = s;
      effect->matrix[9] = 0.0f;
      effect->matrix[10] = c;
      effect->matrix[11] = 0.0f;
      effect->matrix[12] = 0.0f;
      effect->matrix[13] = 0.0f;
      effect->matrix[14] = 0.0f;
      effect->matrix[15] = 1.0f;
      ((GfxEffect *)obj->effect2)->color[3] = 0.0f;
    }
  } else if (typeId == 0x77) {
    obj->effect = GfxEffectSpawn(g_worldEffectMgr, 0xd, obj->base.base.pos);
  }

  if (obj->effect != NULL) {
    rec = (GameGimmickRecord *)obj->base.record;
    heading = (float)(s32)rec->heading * 6.2831855f * 1.5259022e-05f;
    if (!(heading <= 3.1415927f)) {
      heading = heading - 6.2831855f;
    } else if (heading <= -3.1415927f) {
      heading = heading + 6.2831855f;
    }
    effect = (GfxEffect *)obj->effect;
    angle = heading + 3.14f;
    c = __builtin_cosf(angle);
    s = __builtin_sinf(angle);
    effect->matrix[0] = c;
    effect->matrix[1] = 0.0f;
    effect->matrix[2] = -s;
    effect->matrix[3] = 0.0f;
    effect->matrix[4] = 0.0f;
    effect->matrix[5] = 1.0f;
    effect->matrix[6] = 0.0f;
    effect->matrix[7] = 0.0f;
    effect->matrix[8] = s;
    effect->matrix[9] = 0.0f;
    effect->matrix[10] = c;
    effect->matrix[11] = 0.0f;
    effect->matrix[12] = 0.0f;
    effect->matrix[13] = 0.0f;
    effect->matrix[14] = 0.0f;
    effect->matrix[15] = 1.0f;
  }
  return &obj->base.base.base;
}
