// bdc 0x089a6080 UiMainMenuCreateModels
#include "bdc.h"

/* Creates the main menu's 3D scene (phase 0, from `UiMainMenuPhaseLoad`):
   - clears the five orbit records `items` and the 0x28 bytes from `baseState`;
   - initialises the camera (`camera`): target (0,0,0,0), eye (0, 80, 200, 0), full update;
   - the base `menu_daiza.gmo` (`baseModel`, specular 0.6 grey power 8): its node `_02_base`
     (`baseNode`, `baseMatrix` = its local matrix) is set to a rotation about X by -pi/2, then
     rotated about Y by `baseAngle` = cursor * 1.256, keeping its translation row; ambient alpha 0;
   - per item i (0..4): `items[i].from.slot` = `UiMainMenuItemDistance`(i, cursor), then model i:
     0 `menu_worldmap.gmo` (scale 0.5, y 25);
     1 the Bakugan `g_btlModelNames[1]` (motion enabled; its first motion of
       `g_charMotionNameLists[1]`, loaded from `"<name>.gmo"` when not registered, played looping
       with blend 0.2; virtual update; `base.unk08` = 1, `BtlBakuganSetupModelShading`, scale 0.35,
       turned about Y by -0.6 and about X by 0.2, y 5, motion speed 1);
     2 `menu_gauntlet.gmo` (scale 0.38, y 40, motion speed 1);
     3 `menu_credit.gmo` (rotation about Y by 0.0975, scale 0.35, y 27, motion speed 1);
     4 `menu_itembox.gmo` (rotation about Y by 0.25, scale 0.6, y 14; loads the open/close motions
       of `UiItemBoxGetMotionName` 0/1, plays open when the cursor is on item 4 (light mode 1)
       or close otherwise (light mode 0) at frame 0.2, motion speed 200, virtual update).
     Each created model then gets lighting on, specular 0.4 grey power 8, its position saved to
     `itemHome[i]`, `items[i].angle` = `UiWrapAngle`(slot * 1.256 + 1.57), x/z from
     `UiOrbitPoint`(angle, itemHome[i][0], itemHome[i][2], 0, 0, radius 70), the position copied
     into the GMO root matrix translation row (w = 1) and ambient alpha 0.
   Models are allocated with `MemAlloc` (0x140 bytes, low end of the heap) and built with
   `GfxModelCtor`; a failed allocation leaves the slot NULL but the per-item setup still reads
   through it (as the original does).
   The rotations are VFPU `vrot` of angle * S703 (bank 2/pi), i.e. cos/sin of the angle in radians;
   the multiplies are `vmmul.q E200, E100, E000` = rot * m (column-major). */

/* Inlined in the original (once per model): allocate from the low end of the heap and construct. */
static inline GfxModel *UiMainMenuNewModel(const char *gmoName)
{
  GfxModel *mem;
  GfxModel *model;
  bool fromLow;

  model = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = (GfxModel *)MemAlloc(sizeof(GfxModel), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    GfxModelCtor(mem, gmoName, 0);
    model = mem;
  }
  return model;
}

/* rot = rotation about Y by `angle` radians, fields as columns (vrot of angle * S703 in quarter turns):
   x = (c, 0, -s, 0), y = (0, 1, 0, 0), z = (s, 0, c, 0), w = (0, 0, 0, 1). */
static void UiMainMenuRotY(float *rot, float angle)
{
  float c = __builtin_cosf(angle);
  float s = __builtin_sinf(angle);

  rot[0] = c;    rot[1] = 0.0f;  rot[2] = -s;   rot[3] = 0.0f;
  rot[4] = 0.0f; rot[5] = 1.0f;  rot[6] = 0.0f; rot[7] = 0.0f;
  rot[8] = s;    rot[9] = 0.0f;  rot[10] = c;   rot[11] = 0.0f;
  rot[12] = 0.0f; rot[13] = 0.0f; rot[14] = 0.0f; rot[15] = 1.0f;
}

/* rot = rotation about X by `angle` radians: x = (1, 0, 0, 0), y = (0, c, s, 0), z = (0, -s, c, 0),
   w = (0, 0, 0, 1). */
static void UiMainMenuRotX(float *rot, float angle)
{
  float c = __builtin_cosf(angle);
  float s = __builtin_sinf(angle);

  rot[0] = 1.0f;  rot[1] = 0.0f;  rot[2] = 0.0f;  rot[3] = 0.0f;
  rot[4] = 0.0f;  rot[5] = c;     rot[6] = s;     rot[7] = 0.0f;
  rot[8] = 0.0f;  rot[9] = -s;    rot[10] = c;    rot[11] = 0.0f;
  rot[12] = 0.0f; rot[13] = 0.0f; rot[14] = 0.0f; rot[15] = 1.0f;
}

/* m = rot * m, column-major (`vmmul.q E200, E100, E000` with m in M100, rot in M000):
   column j of the result = sum over k of m[j][k] * column k of rot, summed left to right. */
static void UiMainMenuMulRot(float *m, const float *rot)
{
  float r[16];
  int j;
  int row;

  for (j = 0; j < 4; j++) {
    for (row = 0; row < 4; row++) {
      r[4 * j + row] = m[4 * j + 0] * rot[0 + row] + m[4 * j + 1] * rot[4 + row]
                       + m[4 * j + 2] * rot[8 + row] + m[4 * j + 3] * rot[12 + row];
    }
  }
  for (j = 0; j < 16; j++) {
    m[j] = r[j];
  }
}

/* 16-byte copy (lv.q/sv.q pair). */
static void UiMainMenuCopyQuad(float *dst, const float *src)
{
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

void UiMainMenuCreateModels(UiMainMenu *self)
{
  float baseSpecular[4];
  char path[64];
  char openName[64];
  char closeName[64];
  float savedPos[4];
  float itemSpecular[4];
  float orbit[4];
  float rot[16];
  float angle;
  GfxCamera *cam;
  GfxModel *model;
  GmoNode *node;
  GmoModel *data;
  const GfxModelVtable *vt;
  char **motionNames;
  float *m;
  s32 index;
  int i;

  memset(self->items, 0, 200);
  memset(self->baseState, 0, 0x28);
  cam = (GfxCamera *)self->camera;
  GfxCameraInit(cam);
  cam->target[0] = 0.0f;
  cam->target[1] = 0.0f;
  cam->target[2] = 0.0f;
  cam->target[3] = 0.0f;
  cam->eye[0] = 0.0f;
  cam->eye[1] = 80.0f;
  cam->eye[3] = 0.0f;
  cam->eye[2] = 200.0f;
  GfxCameraUpdate(cam, 0xffffffffu);

  model = UiMainMenuNewModel("menu_daiza.gmo");
  self->baseModel = model;
  baseSpecular[0] = 0.6f;
  baseSpecular[1] = 0.6f;
  baseSpecular[2] = 0.6f;
  baseSpecular[3] = 1.0f;
  GfxModelSetSpecular(8.0f, model, baseSpecular, NULL);
  node = (GmoNode *)GfxModelFindNode((GfxModel *)self->baseModel, "_02_base");
  self->baseNode = node;
  self->baseMatrix = node->localMatrix;
  UiMainMenuCopyQuad(savedPos, &node->localMatrix[12]);
  /* rotation about X by -pi/2 (0xbfc90fdb) */
  UiMainMenuRotX(self->baseMatrix, -1.57079637f);
  angle = (float)self->cursor * 1.256f;
  self->baseAngle = angle;
  UiMainMenuRotY(rot, angle);
  UiMainMenuMulRot(self->baseMatrix, rot);
  UiMainMenuCopyQuad(&self->baseMatrix[12], savedPos);
  ((GfxModel *)self->baseModel)->ambient[3] = 0.0f;

  for (i = 0; i < 5; i++) {
    self->items[i].from.slot = UiMainMenuItemDistance(self, (u8)i, (u8)self->cursor);
    switch (i) {
    case 1:
      model = UiMainMenuNewModel(g_btlModelNames[1]);
      self->models[i] = model;
      GfxModelEnableMotion(model);
      motionNames = g_charMotionNameLists[1];
      if (motionNames != NULL) {
        index = GmoMotionIndexOfName(GmoMotionMgrGet(), motionNames[0]);
        if (index == -1) {
          sprintf(path, "%s.gmo", motionNames[0]);
          GmoMotionLoadFile(GmoMotionMgrGet(), path);
        }
        model = (GfxModel *)self->models[i];
        index = GmoMotionIndexOfName(GmoMotionMgrGet(), motionNames[0]);
        GfxModelPlayMotion(0.2f, model, index, 1);
      }
      model = (GfxModel *)self->models[i];
      vt = (const GfxModelVtable *)model->base.vtable;
      vt->update((char *)model + vt->updateAdjust);
      ((GfxModel *)self->models[i])->base.unk08 = 1;
      BtlBakuganSetupModelShading(self->models[i]);
      data = ((GfxModel *)self->models[i])->data;
      data->rootMatrix[10] = 0.35f;
      data->rootMatrix[5] = 0.35f;
      data->rootMatrix[0] = 0.35f;
      UiMainMenuRotY(rot, -0.6f);
      UiMainMenuMulRot(((GfxModel *)self->models[i])->data->rootMatrix, rot);
      UiMainMenuRotX(rot, 0.2f);
      UiMainMenuMulRot(((GfxModel *)self->models[i])->data->rootMatrix, rot);
      ((GfxModel *)self->models[i])->pos[1] = 5.0f;
      model = (GfxModel *)self->models[i];
      vt = (const GfxModelVtable *)model->base.vtable;
      vt->setMotionSpeed((char *)model + vt->setMotionSpeedAdjust, 1.0f);
      break;
    case 2:
      model = UiMainMenuNewModel("menu_gauntlet.gmo");
      self->models[i] = model;
      data = model->data;
      data->rootMatrix[10] = 0.38f;
      data->rootMatrix[5] = 0.38f;
      data->rootMatrix[0] = 0.38f;
      ((GfxModel *)self->models[i])->pos[1] = 40.0f;
      model = (GfxModel *)self->models[i];
      vt = (const GfxModelVtable *)model->base.vtable;
      vt->setMotionSpeed((char *)model + vt->setMotionSpeedAdjust, 1.0f);
      break;
    case 3:
      model = UiMainMenuNewModel("menu_credit.gmo");
      self->models[i] = model;
      UiMainMenuRotY(model->data->rootMatrix, 0.0975f);
      data = ((GfxModel *)self->models[i])->data;
      data->rootMatrix[10] = 0.35f;
      data->rootMatrix[5] = 0.35f;
      data->rootMatrix[0] = 0.35f;
      ((GfxModel *)self->models[i])->pos[1] = 27.0f;
      model = (GfxModel *)self->models[i];
      vt = (const GfxModelVtable *)model->base.vtable;
      vt->setMotionSpeed((char *)model + vt->setMotionSpeedAdjust, 1.0f);
      break;
    case 4:
      model = UiMainMenuNewModel("menu_itembox.gmo");
      self->models[i] = model;
      UiMainMenuRotY(model->data->rootMatrix, 0.25f);
      data = ((GfxModel *)self->models[i])->data;
      data->rootMatrix[10] = 0.6f;
      data->rootMatrix[5] = 0.6f;
      data->rootMatrix[0] = 0.6f;
      ((GfxModel *)self->models[i])->pos[1] = 14.0f;
      UiItemBoxGetMotionName(0, openName);
      sprintf(path, "%s.gmo", openName);
      GmoMotionLoadFile(GmoMotionMgrGet(), path);
      UiItemBoxGetMotionName(1, closeName);
      sprintf(path, "%s.gmo", closeName);
      GmoMotionLoadFile(GmoMotionMgrGet(), path);
      GfxModelEnableMotion((GfxModel *)self->models[i]);
      if (self->cursor == 4) {
        GfxModelPlayMotionByName(0.2f, (GfxModel *)self->models[i], openName, false);
        UiMainMenuSetLightMode(self, 1);
      } else {
        GfxModelPlayMotionByName(0.2f, (GfxModel *)self->models[i], closeName, false);
        UiMainMenuSetLightMode(self, 0);
      }
      model = (GfxModel *)self->models[i];
      vt = (const GfxModelVtable *)model->base.vtable;
      vt->setMotionSpeed((char *)model + vt->setMotionSpeedAdjust, 200.0f);
      model = (GfxModel *)self->models[i];
      vt = (const GfxModelVtable *)model->base.vtable;
      vt->update((char *)model + vt->updateAdjust);
      break;
    default:
      model = UiMainMenuNewModel("menu_worldmap.gmo");
      self->models[i] = model;
      data = model->data;
      data->rootMatrix[10] = 0.5f;
      data->rootMatrix[5] = 0.5f;
      data->rootMatrix[0] = 0.5f;
      ((GfxModel *)self->models[i])->pos[1] = 25.0f;
      break;
    }
    if (self->models[i] != NULL) {
      ((GfxModel *)self->models[i])->lighting = 1;
      itemSpecular[0] = 0.4f;
      itemSpecular[1] = 0.4f;
      itemSpecular[2] = 0.4f;
      itemSpecular[3] = 1.0f;
      GfxModelSetSpecular(8.0f, (GfxModel *)self->models[i], itemSpecular, NULL);
      UiMainMenuCopyQuad(self->itemHome[i], ((GfxModel *)self->models[i])->pos);
      angle = UiWrapAngle((float)self->items[i].from.slot * 1.256f + 1.57f);
      self->items[i].angle = angle;
      UiOrbitPoint(angle, self->itemHome[i][0], self->itemHome[i][2], 0.0f, 0.0f, orbit, 0x46);
      ((GfxModel *)self->models[i])->pos[0] = orbit[0];
      ((GfxModel *)self->models[i])->pos[2] = orbit[1];
      model = (GfxModel *)self->models[i];
      UiMainMenuCopyQuad(&model->data->rootMatrix[12], model->pos);
      ((GfxModel *)self->models[i])->data->rootMatrix[15] = 1.0f;
      ((GfxModel *)self->models[i])->ambient[3] = 0.0f;
    }
  }
}
