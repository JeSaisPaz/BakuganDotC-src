// bdc 0x088a4844 ActorStageObjEggCrystalBreak
#include "bdc.h"

/* Break virtual of the egg crystal (vtable `0x08af2524` slot 11): does nothing while task 0x14a
   runs. Otherwise flags the companion unit `+0x320` dead (`combat.dead`), marks its layout spawn
   record `+0x154` done and releases it (`ActorStageObjReleaseRecord`), drops the model onto the
   ground (`CollisionFindGroundPoint`) and, in battle, builds the debris model
   `fz_crystal02_break_fbx.gmo` (`ActorStageObjDebrisCtor`, appended to the camera task's list
   `+0x468` for 30 frames, translucent materials, element-tinted), plays sound 0x200259 and launches
   the fragments with `debrisVel` (`ActorStageObjDebrisLaunch`). While the battle is undecided
   (`g_btlBattleOutcome` == 0) it then moves the crystal 5000 units down, moves the companion's
   anchor matrix along and disables its two colliders. Finally clears the alpha (`ambient[3]`,
   `fade`) and queues itself for deletion (`CoreObjectDeferDelete`). */

void ActorStageObjEggCrystalBreak(ActorStageObjEggCrystal *self)
{
  float *pos;
  float y;
  int i;
  bool fromLow;
  void *mem;
  ActorStageObjDebris *debris;
  BtlMain *camTask;
  GfxMaterialState *mat;
  BtlBakugan *unit;
  CollisionSphere *sphere;
  const VtblEntry *e;
  float ground[4] __attribute__((aligned(16)));
  float probe[4] __attribute__((aligned(16)));
  float colour[4] __attribute__((aligned(16)));
  float tint[4] __attribute__((aligned(16)));

  if (CoreTaskExists(0x14a) != 0) {
    return;
  }
  pos = self->base.base.pos;
  if (self->unit != NULL) {
    ((BtlBakugan *)self->unit)->combat.dead = 1;
  }
  if (self->base.record != NULL) {
    ((ActorStageObjRecord *)self->base.record)->doneFlags[0] = 1;
    ActorStageObjReleaseRecord(self->base.record);
    self->base.record = NULL;
  }
  /* ground point below the position raised by 1000 */
  for (i = 0; i < 4; i++) {
    probe[i] = pos[i];
  }
  probe[1] = probe[1] + 1000.0f;
  CollisionFindGroundPoint(ground, probe, 0x3fbf2500);
  for (i = 0; i < 4; i++) {
    probe[i] = ground[i];
  }
  y = probe[1];
  for (i = 0; i < 4; i++) {
    self->base.base.data->rootMatrix[12 + i] = probe[i];
  }

  if (BtlCameraTaskExists() != 0) {
    debris = NULL;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(ActorStageObjDebris), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
      ActorStageObjDebrisCtor(mem, "fz_crystal02_break_fbx.gmo");
      debris = (ActorStageObjDebris *)mem;
    }
    if (SndHasListener()) {
      SndEmitterCreateAtPos(SndGetListener(), 0x200259, &self->base.base.data->rootMatrix[12], 0, 1);
    }
    ActorStageObjDebrisLaunch(y, y, debris, self->base.base.data->rootMatrix, self->debrisVel);
    camTask = (BtlMain *)BtlGetCameraTask();
    CoreObjectListAppend(&debris->base.base, &camTask->modelLists[1]);
    debris->lifetime = 30;
    GfxModelForEachMaterial(&debris->base, (void *)ActorStageObjEggCrystalMaterialSetTranslucent, NULL);
    mat = GfxModelFindMaterialState(&debris->base, "fz_crystal02_break__Z1");
    if (mat != NULL) {
      mat->renderFlags = (mat->renderFlags & 0x3f) | 0x40;
      mat->shadeFlags = mat->shadeFlags & 0xfc;
      mat->shadeFlags = (mat->shadeFlags & 0x1f) | 0xa0;
      mat->renderFlags = (mat->renderFlags & 0xfc) | 0x02;
    }
    mat = GfxModelFindMaterialState(&debris->base, "fz_crystal02_break__BA_Z2");
    if (mat != NULL) {
      mat->renderFlags = (mat->renderFlags & 0x3f) | 0x80;
      mat->shadeFlags = (mat->shadeFlags & 0xfc) | 0x02;
    }
    if (self->element != 0) {
      ActorCrystalGetStyleColor(colour, self->element);
      for (i = 0; i < 4; i++) {
        tint[i] = colour[i];
      }
      GfxModelSetAmbientColor(&debris->base, tint, NULL);
    }
  }

  if (BtlCameraTaskExists() != 0 && g_btlBattleOutcome == 0) {
    pos[1] = pos[1] - 5000.0f;
    for (i = 0; i < 4; i++) {
      self->base.base.data->rootMatrix[12 + i] = pos[i];
    }
    if (self->unit != NULL) {
      unit = (BtlBakugan *)self->unit;
      /* anchor matrix = model root matrix */
      for (i = 0; i < 16; i++) {
        unit->anchorMatrix[i / 4][i % 4] = self->base.base.data->rootMatrix[i];
      }
      if (unit->collider0 != NULL) {
        unit->collider0->hitTimer = 0;
        unit->collider0->flags |= 1;
        unit->collider0->flags |= 0x40;
        unit->collider0->flags |= 4;
        unit->collider0->attachDirty = 1;
      }
      if (unit->collider1 != NULL) {
        unit->collider1->flags |= 1;
        unit->collider1->hitTimer = 0;
        unit->collider1->flags |= 0x40;
        unit->collider1->flags |= 4;
        /* sphere centre (and the radius² slot) = model translation, centre Y raised by the radius */
        sphere = (CollisionSphere *)unit->collider1->shapeDesc;
        sphere->center[0] = self->base.base.data->rootMatrix[12];
        sphere->center[1] = self->base.base.data->rootMatrix[13];
        sphere->center[2] = self->base.base.data->rootMatrix[14];
        sphere->radiusSq = self->base.base.data->rootMatrix[15];
        sphere->center[1] = sphere->center[1] + sphere->radius;
        /* slot 9: CollisionSphereRecalc (no VFPU use) */
        e = &sphere->vtbl[9];
        ((void (*)(void *))e->fn)((u8 *)sphere + e->delta);
      }
    }
  }
  self->base.base.ambient[3] = 0.0f;
  self->base.fade = 0.0f;
  CoreObjectDeferDelete(&self->base.base.base, 0);
}
