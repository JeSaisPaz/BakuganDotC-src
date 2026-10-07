// bdc 0x088a3554 ActorCrystalStandCtor
#include "bdc.h"

/* Constructor of the 0x150-byte `ActorCrystalStand` model that sits under a crystal
   (`ActorCrystalCtor`, `ActorCrystalStandSpawnAtPoint`): runs the GMO model constructor
   (`GfxModelCtor(stand, gmoName, 0xc00)`), installs `g_actorCrystalStandVtable`, hooks material
   `fz_crystal01_stand_01` and, if the matching `<name>_col.ctc` collision file exists in the loaded
   pack chain, creates a mesh collider from it that follows the model's root matrix. Finally enables
   lighting and applies the arena light colours. Returns `stand`. */

void *ActorCrystalStandCtor(void *stand, const char *gmoName)

{
  ActorCrystalStand *self = stand;
  CollisionCollider *collider;
  CollisionCollider *mem;
  char *ext;
  bool fromLow;
  char colName[128];

  GfxModelCtor(&self->base, gmoName, 0xc00);
  self->base.base.vtable = g_actorCrystalStandVtable;
  GfxModelSetMaterialAnimCallback(&self->base, "fz_crystal01_stand_01",
                                  ActorCrystalStandMaterialWriteNoSpecular, NULL);
  GfxModelForEachMaterial(&self->base, ActorCrystalStandMaterialSetTranslucent, NULL);
  self->collider = NULL;
  strcpy(colName, gmoName);
  ext = strrchr(colName, '.');
  if (ext != NULL) {
    *ext = '\0';
  }
  strcat(colName, "_col.ctc");
  if (CorePackChainFind(g_ioLzsPackages, colName) != NULL) {
    collider = NULL;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(CollisionCollider), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
      CollisionColliderCtor(&mem->node, 2);
      collider = mem;
    }
    self->collider = collider;
    CollisionColliderInitMesh((CoreNode *)collider, CorePackChainFind(g_ioLzsPackages, colName), 9,
                              self, 0);
    self->collider->byte104 = 0;
    self->collider->hitField144 = 1;
    self->collider->owner = self;
    collider = self->collider;
    collider->attachMatrix = (float (*)[4])self->base.data->rootMatrix;
    collider->attachDirty = 1;
    collider = self->collider;
    collider->hitTimer = -1;
    collider->flags |= 1;
  }
  self->base.lighting = 1;
  BtlStageApplyLightColors(self);
  return stand;
}
