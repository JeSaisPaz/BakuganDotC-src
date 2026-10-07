// bdc 0x088d8f9c GameGimmickCtor
#include "bdc.h"

/* Base constructor of the field gimmick objects (`GMKOBJ_*`: switches, barriers, cameras, IR
   sensors, steam, drums, poles, pass-code switches, item boxes, core points, collection box,
   Marucho jet door): builds the model `g_gameGimmickKindTable``[kind][0]` (`GfxModelCtor`),
   runs the inlined secondary-base constructor (vtable `g_gameGimmickSubBaseVtbl`, clears
   `+0x140..+0x15f`, `active = 1`), installs the gimmick vtables `g_gameGimmickVtbl` /
   `g_gameGimmickVtbl2`, stores the layout record, `kind`, `typeId` and `flag`, sets the object
   kind `unk08 = 0x87` and appends it to the field task's gimmick list (`GameFieldTask.gimmicks`,
   scanned by `GameFieldCheckTriggers`). Then it sets `rot = (0, heading, 0, 0)` (record heading
   in 1/65536 turns, wrapped to (-pi, pi]), multiplies the model's root matrix by the Y rotation
   (VFPU `vrot`/`vmmul`, written out per element), sets `pos` to the record position (20.12) times
   20 with `w = 0`, `alpha = 1` and clears `hidden`. Returns `obj`. */

CoreObject *GameGimmickCtor(GameGimmick *obj, s32 kind, void *record, u16 typeId, u8 flag)
{
  GameFieldTask *field;
  GameGimmickRecord *rec;
  GmoModel *model;
  float heading;
  float c;
  float s;
  float sum;
  float rotY[4][4];
  float result[16];
  float scaled[3];
  int j;
  int r;
  int k;

  GfxModelCtor(&obj->base, g_gameGimmickKindTable[kind][0], 0);
  obj->vtbl2 = g_gameGimmickSubBaseVtbl;
  obj->subByte140 = 0;
  obj->flag = flag;
  obj->typeId = typeId;
  obj->subHalf15c = 0;
  obj->active = 1;
  obj->contactFlags = 0;
  memset(obj->subBlock144, 0, 0xc);
  memset(obj->subBlock150, 0, 0xc);
  obj->base.base.vtable = g_gameGimmickVtbl;
  obj->vtbl2 = g_gameGimmickVtbl2;
  obj->record = record;
  obj->base.base.unk08 = 0x87;
  obj->kind = kind;
  field = (GameFieldTask *)GameFieldFindTask();
  CoreObjectListAppend(&obj->base.base, (CoreObjectList *)&field->gimmicks);

  /* The binary wraps all three angles the same way; x and z are the constant 0.0f, which no
     branch changes. */
  rec = (GameGimmickRecord *)obj->record;
  heading = (float)(s32)rec->heading * 6.28318548f * 1.52590219e-05f;
  if (!(heading <= 3.14159274f)) {
    heading = heading - 6.28318548f;
  } else if (heading <= -3.14159274f) {
    heading = heading + 6.28318548f;
  }
  obj->base.rot[0] = 0.0f;
  obj->base.rot[1] = heading;
  obj->base.rot[2] = 0.0f;
  obj->base.rot[3] = 0.0f;

  /* Y rotation of the heading (VFPU `vrot` of heading * 2/pi, quarter turns), rows of the 4x4
     matrix whose columns the `vrot`/`vidt` build. */
  model = obj->base.data;
  heading = obj->base.rot[1];
  c = __builtin_cosf(heading);
  s = __builtin_sinf(heading);
  rotY[0][0] = c;     rotY[0][1] = 0.0f; rotY[0][2] = s;    rotY[0][3] = 0.0f;
  rotY[1][0] = 0.0f;  rotY[1][1] = 1.0f; rotY[1][2] = 0.0f; rotY[1][3] = 0.0f;
  rotY[2][0] = -s;    rotY[2][1] = 0.0f; rotY[2][2] = c;    rotY[2][3] = 0.0f;
  rotY[3][0] = 0.0f;  rotY[3][1] = 0.0f; rotY[3][2] = 0.0f; rotY[3][3] = 1.0f;
  /* `vmmul.q E200, E100, E000` (S operand displayed transposed): M200 = M000 * M100 = rotY * root,
     so new root[j][r] = sum_k rotY[r][k] * old root[j][k] (root stored column per 16 bytes). */
  for (j = 0; j < 4; j++) {
    for (r = 0; r < 4; r++) {
      sum = rotY[r][0] * model->rootMatrix[j * 4 + 0];
      for (k = 1; k < 4; k++) {
        sum = sum + rotY[r][k] * model->rootMatrix[j * 4 + k];
      }
      result[j * 4 + r] = sum;
    }
  }
  for (j = 0; j < 16; j++) {
    model->rootMatrix[j] = result[j];
  }

  /* pos = record position (20.12) * 20; `w` is the bank's S713 = 0 (`vscl.t` leaves that lane). */
  rec = (GameGimmickRecord *)obj->record;
  scaled[0] = (float)rec->pos[0] * 0.000244140625f;
  scaled[1] = (float)rec->pos[1] * 0.000244140625f;
  scaled[2] = (float)rec->pos[2] * 0.000244140625f;
  obj->base.pos[0] = scaled[0] * 20.0f;
  obj->base.pos[1] = scaled[1] * 20.0f;
  obj->base.pos[2] = scaled[2] * 20.0f;
  obj->base.pos[3] = 0.0f;
  obj->alpha = 1.0f;
  obj->hidden = 0;
  return &obj->base.base;
}
