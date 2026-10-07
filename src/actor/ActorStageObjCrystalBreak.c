// bdc 0x088b4b00 ActorStageObjCrystalBreak
#include "bdc.h"

/* Break virtual (vtable `0x08af2b94` slot 11) of the crystal stage object: unless task 0x14a runs,
   flags the companion unit `+800` dead (`combat.dead`), marks its layout spawn record `+0x154`
   done (`doneFlags[0]`) and releases it (`ActorStageObjReleaseRecord`), drops the model onto the
   ground (`CollisionFindGroundPoint`) and, in battle, builds the debris model
   `little_crystal_bk.gmo` (`ActorStageObjDebrisCtor`, appended to the camera task's list
   `+0x468`, 30 frames), plays sound `0x200259`, launches the fragments
   (`ActorStageObjDebrisLaunch`), sets the materials `gimmick_11_littlecrystal_break` /
   `gimmick_11_littlecrystalbreak` blend bits and tints it with the element `+0x32c`
   (`GfxModelSetAmbientColor`). Unless the battle is won (`BtlMainCheckWin`) it then moves the
   crystal 5000 units down and moves the companion's anchor and colliders with it (collider flags
   `|= 1|0x40|4`, hit timer 0). Clears the alpha `+0x6c` and `+0x228`. */

void ActorStageObjCrystalBreak(ActorStageObjCrystal *self)
{
  BtlBakugan *unit;
  ActorStageObjDebris *debris;
  void *mem;
  bool fromLow;
  float groundY;
  GfxMaterialState *mat;
  BtlMain *camTask;
  CollisionCollider *collider;
  CollisionSphere *sphere;
  const VtblEntry *update;
  int i;
  float ground[4] __attribute__((aligned(16)));
  float probe[4] __attribute__((aligned(16)));
  float styleColour[4] __attribute__((aligned(16)));
  float colour[4] __attribute__((aligned(16)));

  if (CoreTaskExists(0x14a) != 0) {
    return;
  }
  if (self->unit != NULL) {
    ((BtlBakugan *)self->unit)->combat.dead = 1;
  }
  if (self->base.record != NULL) {
    ((ActorStageObjRecord *)self->base.record)->doneFlags[0] = 1;
    ActorStageObjReleaseRecord(self->base.record);
    self->base.record = NULL;
  }
  for (i = 0; i < 4; i++) {
    probe[i] = self->base.base.pos[i];
  }
  probe[1] = probe[1] + 1000.0f;
  CollisionFindGroundPoint(ground, probe, 0x3fbf2500);
  probe[0] = ground[0];
  probe[1] = ground[1];
  probe[2] = ground[2];
  probe[3] = ground[3];
  groundY = probe[1];
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
      ActorStageObjDebrisCtor(mem, "little_crystal_bk.gmo");
      debris = (ActorStageObjDebris *)mem;
    }
    if (SndHasListener()) {
      SndEmitterCreateAtPos(SndGetListener(), 0x200259, &self->base.base.data->rootMatrix[12], 0, 1);
    }
    ActorStageObjDebrisLaunch(groundY, groundY, debris, self->base.base.data->rootMatrix,
                              self->knockDir);
    camTask = (BtlMain *)BtlGetCameraTask();
    CoreObjectListAppend(&debris->base.base, &camTask->modelLists[1]);
    debris->lifetime = 30;
    GfxModelForEachMaterial(&debris->base, (void *)ActorStageObjCrystalMaterialSetTranslucent, NULL);
    mat = GfxModelFindMaterialStateBySubstr(&debris->base, "gimmick_11_littlecrystal_break");
    if (mat != NULL) {
      mat->renderFlags = (mat->renderFlags & 0x3f) | 0x40;
      mat->shadeFlags = mat->shadeFlags & 0xfc;
      mat->shadeFlags = (mat->shadeFlags & 0x1f) | 0x20;
      mat->renderFlags = (mat->renderFlags & 0xfc) | 0x02;
    }
    mat = GfxModelFindMaterialStateBySubstr(&debris->base, "gimmick_11_littlecrystalbreak");
    if (mat != NULL) {
      mat->renderFlags = (mat->renderFlags & 0x3f) | 0x80;
      mat->shadeFlags = (mat->shadeFlags & 0xfc) | 0x02;
    }
    if (self->element != 0) {
      ActorCrystalGetStyleColor(styleColour, self->element);
      for (i = 0; i < 4; i++) {
        colour[i] = styleColour[i];
      }
      GfxModelSetAmbientColor(&debris->base, colour, NULL);
    }
  }
  if (BtlCameraTaskExists() != 0 && BtlMainCheckWin((BtlMain *)BtlGetCameraTask()) == 0) {
    self->base.base.pos[1] = self->base.base.pos[1] - 5000.0f;
    for (i = 0; i < 4; i++) {
      self->base.base.data->rootMatrix[12 + i] = self->base.base.pos[i];
    }
    if (self->unit != NULL) {
      unit = (BtlBakugan *)self->unit;
      for (i = 0; i < 16; i++) {
        unit->anchorMatrix[i / 4][i % 4] = self->base.base.data->rootMatrix[i];
      }
      if (((BtlBakugan *)self->unit)->collider0 != NULL) {
        collider = ((BtlBakugan *)self->unit)->collider0;
        collider->hitTimer = 0;
        collider->flags = collider->flags | 1;
        collider = ((BtlBakugan *)self->unit)->collider0;
        collider->flags = collider->flags | 0x40;
        collider = ((BtlBakugan *)self->unit)->collider0;
        collider->flags = collider->flags | 4;
        ((BtlBakugan *)self->unit)->collider0->attachDirty = 1;
      }
      if (((BtlBakugan *)self->unit)->collider1 != NULL) {
        collider = ((BtlBakugan *)self->unit)->collider1;
        collider->flags = collider->flags | 1;
        collider->hitTimer = 0;
        collider = ((BtlBakugan *)self->unit)->collider1;
        collider->flags = collider->flags | 0x40;
        collider = ((BtlBakugan *)self->unit)->collider1;
        collider->flags = collider->flags | 4;
        sphere = (CollisionSphere *)((BtlBakugan *)self->unit)->collider1->shapeDesc;
        /* centre + radiusSq <- model translation (one quad copy) */
        for (i = 0; i < 3; i++) {
          sphere->center[i] = self->base.base.data->rootMatrix[12 + i];
        }
        sphere->radiusSq = self->base.base.data->rootMatrix[15];
        sphere->center[1] = sphere->center[1] + sphere->radius;
        update = &sphere->vtbl[9];
        ((void (*)(void *))update->fn)((u8 *)sphere + update->delta);
      }
    }
  }
  self->base.base.ambient[3] = 0.0f;
  self->base.fade = 0.0f;
}
