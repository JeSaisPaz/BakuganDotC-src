// bdc 0x0885ef14 BtlBakuganCtor
#include "bdc.h"

/* Constructor of the battle Bakugan unit class (object size 0x670 when built by
   `BtlCreateBakugan` for the 20 playable Bakugan, `kind` 1..0x14; also the base constructor of
   `ActorCrystalCtor` and `BtlCpuUnitCtor`): loads the model `g_btlModelNames``[kind]` as a
   `GfxModelCtor` object, installs `g_btlBakuganVtbl` and the kinds/vtables of the embedded
   body capsule (with its segment view) and push sphere, constructs the embedded combat state,
   default-initialises the fields, assigns the unit id and the stencil/animation phase, copies
   radius/height/gravity/scale from the stat table, caches the `Bip01_Spine` node (node 1 as
   fallback), the per-kind attack motion and combo tables, builds the stats record, the input
   controller (0x70 bytes) and, for kinds 1..0x20, the shadow (0x30 bytes, size `reachRadius *
   2.5`) and colliders, then the motions, appends the unit to `g_btlBakuganList`, enters state 0
   and sets flag 0x1000 when `SaveGetProfileFlag0` is set or in rule mode 1 on arena 10.
   Returns `self`. If the
   shadow allocation fails the shadow size is still written through the NULL pointer, as in the
   binary. */

void *BtlBakuganCtor(BtlBakugan *self, s32 kind)

{
  float scaleVec[4] __attribute__((aligned(16)));
  BtlUnitStatTable *stats;
  BtlInput *inputMem;
  BtlInput *input;
  BtlShadow *shadowMem;
  BtlShadow *shadow;
  BtlStats *unitStats;
  bool fromLow;
  float scale;
  float size;

  GfxModelCtor(&self->base, g_btlModelNames[kind], 0);
  self->base.base.vtable = g_btlBakuganVtbl;
  self->bodyShape.vtbl = g_collisionCapsuleVtbl;
  ((SegmentShape *)self->bodyShape.segmentHead)->info = (void *)g_collisionSegmentVtbl;
  ((SegmentShape *)self->bodyShape.segmentHead)->type = 2;
  self->bodyShape.type = 4;
  self->pushShape.vtbl = g_collisionSphereVtbl;
  self->pushShape.type = 3;
  BtlCombatCtor(&self->combat);
  BtlBakuganInitFields(self);
  self->base.base.unk08 = kind;
  self->base.base.id = g_btlNextUnitId++;
  unitStats = BtlStatsCreate();
  self->stats = unitStats;
  if (unitStats != (BtlStats *)0) {
    BtlStatsSetOwner(self->stats, self);
  }
  BtlBakuganInitCombat(self, kind);
  self->stencilRef = g_btlAnimPhaseCounter++ % 0xdf + 0x20;
  stats = self->combat.stats;
  self->radius = stats->bodyRadius;
  self->height = stats->height;
  self->floorMaterial = 0;
  self->gravity = stats->gravity;
  self->spineNode = GfxModelFindNode(&self->base, "Bip01_Spine");
  self->lastHpRatio = BtlCombatGetHpRatio(&self->combat);
  scale = self->combat.stats->modelScale;
  scaleVec[2] = scale;
  scaleVec[1] = scale;
  scaleVec[0] = scale;
  scaleVec[3] = 0.0f;
  self->base.scale[0] = scaleVec[0];
  self->base.scale[1] = scaleVec[1];
  self->base.scale[2] = scaleVec[2];
  self->base.scale[3] = scaleVec[3];
  if (self->spineNode == (void *)0) {
    self->spineNode = GfxModelGetNode(&self->base, 1);
  }
  self->attackMotions = g_btlKindAttackMotions[kind];
  self->combos = g_btlKindComboTables[kind];
  self->motionDone = 0;
  self->hitWindowActive = 0;

  input = (BtlInput *)0;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  inputMem = MemAlloc(0x70, (const char *)0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (inputMem != (BtlInput *)0) {
    BtlInputCtorForUnit(inputMem, self);
    input = inputMem;
  }
  self->input = input;
  GfxModelSetStencilRef(&self->base, (u8)self->stencilRef);
  self->base.fogColor = 0;
  self->base.fogFar = 0.0f;
  self->base.fogNear = 0.0f;
  BtlBakuganSetupModelShading(self);
  BtlBakuganCreateAttachments(self);
  GfxModelCreateSoundObject(&self->base, 8);
  if (self->base.base.unk08 != 0 && self->base.base.unk08 < 0x21) {
    shadow = (BtlShadow *)0;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    shadowMem = MemAlloc(0x30, (const char *)0, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (shadowMem != (BtlShadow *)0) {
      BtlShadowCtorForUnit(shadowMem, self);
      shadow = shadowMem;
    }
    self->shadow = shadow;
    size = self->combat.stats->reachRadius * 2.5f;
    shadow->size[0] = size;
    shadow->size[1] = size;
    shadow->size[2] = size;
    shadow->size[3] = 0.0f;
    BtlBakuganInitColliders(self);
  }
  BtlBakuganInitMotions(self);
  CoreObjectListAppend((CoreObject *)self, g_btlBakuganList);
  BtlBakuganSetState(self, 0, 0);
  if (SaveGetProfileFlag0() != 0 || (g_scriptGlobalVars[8] == 1 && g_btlArenaIndex == 10)) {
    self->flags |= 0x1000;
  }
  return self;
}
