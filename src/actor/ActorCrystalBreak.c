// bdc 0x08856b88 ActorCrystalBreak
#include "bdc.h"

/* Breaks the crystal. In battle (camera task alive) it plays the break sounds `0x20001f` and
   `0x20025c` at the model translation (`SndEmitterCreateAtPos`) and builds the 0x1e0-byte shatter
   model from the pack (`ActorStageObjDebrisCtor`, launched with
   `ActorStageObjDebrisLaunchStrong`): `"fz_crystal01_break_fbx.gmo"` when `barrierVariant` is
   set, otherwise `"fz_crystal01_break_60.gmo"`, for which the crystal's root matrix is first rebuilt
   at scale 1.0 and afterwards at scale 0.6. A built debris model is appended to the camera task's
   `modelLists[1]`, lives 15 frames (battle won) or 45, gets its `"fz_crystal02_break"` material
   blend bits set (additive bits cleared when `additive`), the style colour
   (`GfxModelSetAmbientColor`) and, on stages 0/13 (`GameStageIs0Or13`), a fixed ambient and
   colour. Always: both colliders get hit timer 0 and flags `|= 1|0x40|4`; unless the battle is won
   (or with no camera task) the item is dropped (`BtlBakuganDropItem`), the crystal moved 5000
   units down and virtual entry 23 called. Clears the alpha `ambient[3]`, and in score mode 1
   (`BtlIsScoreMode`) starts regeneration (`regenStep = 1`). */

static inline float ActorCrystalBreakWrapAngle(float a)
{
  if (!(a <= 3.1415927f)) {
    a = a - 6.2831855f;
  }
  else if (a <= -3.1415927f) {
    a = a + 6.2831855f;
  }
  return a;
}

/* rootMatrix = rotY(heading) * diag(scale.xyz, 1), translation <- pos */
static inline void ActorCrystalBreakPlaceModel(ActorCrystal *self, float heading)
{
  float *m = self->base.base.data->rootMatrix;
  const float *scale = self->base.base.scale;
  const float *pos = self->base.base.pos;
  float c = __builtin_cosf(heading);
  float s = __builtin_sinf(heading);

  m[0] = c * scale[0];
  m[1] = 0.0f;
  m[2] = -s * scale[0];
  m[3] = 0.0f;
  m[4] = 0.0f;
  m[5] = scale[1];
  m[6] = 0.0f;
  m[7] = 0.0f;
  m[8] = s * scale[2];
  m[9] = 0.0f;
  m[10] = c * scale[2];
  m[11] = 0.0f;
  m[12] = pos[0];
  m[13] = pos[1];
  m[14] = pos[2];
  m[15] = pos[3];
}

static inline ActorStageObjDebris *ActorCrystalBreakSpawnDebris(ActorCrystal *self,
                                                                const char *name, float floorY,
                                                                float centerY)
{
  ActorStageObjDebris *debris = NULL;
  void *mem;
  bool fromLow;

  if (CorePackChainFindData(g_ioLzsPackages, (char *)name) == NULL) {
    return NULL;
  }
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x1e0, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    ActorStageObjDebrisCtor(mem, name);
    debris = (ActorStageObjDebris *)mem;
  }
  ActorStageObjDebrisLaunchStrong(floorY, centerY, debris, self->base.base.data->rootMatrix, NULL);
  return debris;
}

void ActorCrystalBreak(ActorCrystal *self)
{
  float centerY;
  float floorY;
  ActorStageObjDebris *debris;
  GfxMaterialState *mat;
  const VtblEntry *entry;
  const float *style;
  float colour[4];

  centerY = self->base.base.pos[1];
  floorY = self->base.base.data->rootMatrix[13];
  if (BtlCameraTaskExists() != 0) {
    if (SndHasListener()) {
      SndEmitterCreateAtPos(SndGetListener(), 0x20001f, &self->base.base.data->rootMatrix[12], 0, 1);
    }
    if (SndHasListener()) {
      SndEmitterCreateAtPos(SndGetListener(), 0x20025c, &self->base.base.data->rootMatrix[12], 0, 1);
    }
    if (self->barrierVariant != 0) {
      debris = ActorCrystalBreakSpawnDebris(self, "fz_crystal01_break_fbx.gmo", floorY, centerY);
    }
    else {
      self->base.base.scale[0] = 1.0f;
      self->base.base.scale[1] = 1.0f;
      self->base.base.scale[2] = 1.0f;
      self->base.base.scale[3] = 0.0f;
      ActorCrystalBreakPlaceModel(self, ActorCrystalBreakWrapAngle(1.5707964f - self->base.base.rot[1]));
      debris = ActorCrystalBreakSpawnDebris(self, "fz_crystal01_break_60.gmo", floorY, centerY);
      self->base.base.scale[0] = 0.6f;
      self->base.base.scale[1] = 0.6f;
      self->base.base.scale[2] = 0.6f;
      self->base.base.scale[3] = 0.0f;
      ActorCrystalBreakPlaceModel(self, ActorCrystalBreakWrapAngle(1.5707964f - self->base.base.rot[1]));
    }
    if (debris != NULL) {
      CoreObjectListAppend(&debris->base.base, &((BtlMain *)BtlGetCameraTask())->modelLists[1]);
      if (BtlCameraTaskExists() != 0) {
        if (BtlMainCheckWin((BtlMain *)BtlGetCameraTask()) != 0) {
          debris->lifetime = 15;
        }
        else {
          debris->lifetime = 45;
        }
      }
      mat = (GfxMaterialState *)GfxModelFindMaterialStateBySubstr(&debris->base,
                                                                  "fz_crystal02_break");
      if (mat != NULL) {
        if (self->additive != 0) {
          mat->renderFlags = mat->renderFlags & 0xf3;
        }
        mat->shadeFlags = mat->shadeFlags & 0xfc;
        mat->renderFlags = (mat->renderFlags & 0xfc) | 0x02;
      }
      style = g_actorCrystalStyleColors[self->style];
      colour[0] = style[0];
      colour[1] = style[1];
      colour[2] = style[2];
      colour[3] = style[3];
      GfxModelSetAmbientColor(&debris->base, colour, NULL);
      if (GameStageIs0Or13() != 0) {
        debris->base.ambient[0] = 0.07f;
        debris->base.ambient[1] = 0.02f;
        debris->base.ambient[2] = 0.17f;
        debris->base.ambient[3] = 1.0f;
        debris->base.color[0] = 0.15f;
        debris->base.color[1] = 0.2f;
        debris->base.color[2] = 0.2f;
        debris->base.color[3] = 1.0f;
      }
    }
  }
  self->base.collider0->hitTimer = 0;
  self->base.collider0->flags = self->base.collider0->flags | 1;
  self->base.collider0->flags = self->base.collider0->flags | 0x40;
  self->base.collider0->flags = self->base.collider0->flags | 4;
  self->base.collider1->hitTimer = 0;
  self->base.collider1->flags = self->base.collider1->flags | 1;
  self->base.collider1->flags = self->base.collider1->flags | 0x40;
  self->base.collider1->flags = self->base.collider1->flags | 4;
  if (BtlCameraTaskExists() != 0 && BtlMainCheckWin((BtlMain *)BtlGetCameraTask()) == 0) {
    BtlBakuganDropItem(&self->base);
    self->base.base.pos[1] = self->base.base.pos[1] - 5000.0f;
    self->base.base.data->rootMatrix[12] = self->base.base.pos[0];
    self->base.base.data->rootMatrix[13] = self->base.base.pos[1];
    self->base.base.data->rootMatrix[14] = self->base.base.pos[2];
    self->base.base.data->rootMatrix[15] = self->base.base.pos[3];
    entry = &((const VtblEntry *)self->base.base.base.vtable)[23];
    ((void (*)(void *))entry->fn)((u8 *)self + entry->delta);
  }
  self->base.base.ambient[3] = 0.0f;
  if (BtlIsScoreMode(1) != 0) {
    self->regenStep = 1;
  }
}
