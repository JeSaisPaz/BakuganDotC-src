// bdc 0x088a3d0c ActorStageObjEggCrystalSummon
#include "bdc.h"

/* Creates an egg crystal for a summoning attack (`BtlAttackType80Update`): allocates 0x390 bytes,
   `ActorStageObjEggCrystalCtor` at `pos` (no NULL check afterwards: a failed allocation writes
   through NULL), heading `rot[1] = pos[3]`, HP 100 with gauge, stores the unit kind to hatch
   `hatchKind`, AI level `aiLevel`, owner slot `ownerSlot` (owner = `BtlGetNthCrystal`, linked
   with `ActorStageObjEggCrystalLinkOwner`; `unitFlag` = the owner's `focusCamera`), index
   `ownerIndex`, element `element` and target point `targetPoint`; makes every material translucent
   (`ActorStageObjEggCrystalMaterialSetTranslucent`), marks the `fz_crystal02_Z1` /
   `fz_crystal02_break__BA_Z2` materials, tints by the element when nonzero, sets the stage 0/13
   ambient/colour, starts at alpha 0 in state 1, runs vtable slot 7 once and, when it is the only
   standing egg crystal, script flag 5 is set and `unitFlag` is set, focuses the battle camera on it
   (`BtlCameraFocusUnit``(1300, 200, 40, …)`).
   The vectors are copied with lv.q/sv.q in the original; vtable slot 7 is
   `ActorStageObjEggCrystalUpdate`, which reads no VFPU value from here. */

void ActorStageObjEggCrystalSummon(float *pos, int kind, int element, int aiLevel, int ownerSlot, int index, float *target)
{
  float unitPos[4];
  float posCopy[4];
  float style[4];
  float colour[4];
  float tmp[4];
  ActorStageObjEggCrystal *self;
  ActorStageObjEggCrystal *mem;
  ActorCrystal *owner;
  GfxMaterialState *mat;
  const VtblEntry *slot;
  bool fromLow;

  posCopy[2] = 0.0f;
  posCopy[1] = 0.0f;
  posCopy[0] = 0.0f;
  posCopy[3] = 0.0f;
  posCopy[0] = pos[0];
  posCopy[1] = pos[1];
  posCopy[2] = pos[2];
  posCopy[3] = pos[3];
  mem = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  self = MemAlloc(0x390, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (self != NULL) {
    unitPos[0] = pos[0];
    unitPos[1] = pos[1];
    unitPos[2] = pos[2];
    unitPos[3] = pos[3];
    ActorStageObjEggCrystalCtor(self, unitPos);
    mem = self;
  }
  self = mem;
  self->base.base.rot[1] = posCopy[3];
  self->base.maxHp = 100;
  self->base.hp = 100;
  self->aiLevel = aiLevel;
  self->hatchKind = kind;
  self->ownerSlot = ownerSlot;
  self->ownerIndex = index;
  ActorStageObjEnsureHpGauge(&self->base);
  GfxModelForEachMaterial(&self->base.base, (void *)ActorStageObjEggCrystalMaterialSetTranslucent, NULL);
  self->element = element;
  ActorStageObjEggCrystalSetState(self, 1);
  unitPos[0] = target[0];
  unitPos[1] = target[1];
  unitPos[2] = target[2];
  unitPos[3] = target[3];
  self->targetPoint[0] = unitPos[0];
  self->targetPoint[1] = unitPos[1];
  self->targetPoint[2] = unitPos[2];
  self->targetPoint[3] = unitPos[3];
  owner = (ActorCrystal *)BtlGetNthCrystal(self->ownerSlot);
  self->owner = owner;
  ActorStageObjEggCrystalLinkOwner(self);
  self->unitFlag = owner->focusCamera;

  mat = (GfxMaterialState *)GfxModelFindMaterialState(&self->base.base, "fz_crystal02_Z1");
  if (mat != NULL) {
    mat->shadeFlags = (mat->shadeFlags & ~3) | 2;
    mat->shadeFlags = (mat->shadeFlags & ~0xe0) | 0xa0;
    mat->renderFlags = (mat->renderFlags & ~3) | 2;
  }
  mat = (GfxMaterialState *)GfxModelFindMaterialState(&self->base.base, "fz_crystal02_break__BA_Z2");
  if (mat != NULL) {
    mat->shadeFlags = (mat->shadeFlags & ~3) | 2;
  }
  if (self->element != 0) {
    ActorCrystalGetStyleColor(style, self->element);
    colour[0] = style[0];
    colour[1] = style[1];
    colour[2] = style[2];
    colour[3] = style[3];
    GfxModelSetAmbientColor(&self->base.base, colour, NULL);
  }

  if (GameStageIs0Or13() != 0) {
    tmp[0] = 0.07f;
    tmp[1] = 0.02f;
    tmp[2] = 0.17f;
    tmp[3] = 1.0f;
    self->base.base.ambient[0] = tmp[0];
    self->base.base.ambient[1] = tmp[1];
    self->base.base.ambient[2] = tmp[2];
    self->base.base.ambient[3] = tmp[3];
    tmp[0] = 0.45f;
    tmp[1] = 0.45f;
    tmp[2] = 0.55f;
    tmp[3] = 1.0f;
    self->base.base.color[0] = tmp[0];
    self->base.base.color[1] = tmp[1];
    self->base.base.color[2] = tmp[2];
    self->base.base.color[3] = tmp[3];
  }

  slot = &((const VtblEntry *)self->base.base.base.vtable)[7];
  self->base.base.ambient[3] = 0.0f;
  ((void (*)(void *))slot->fn)((char *)mem + slot->delta);

  if (ActorStageObjCountStandingEggCrystals() == 1 && CoreBitsetTest(5, g_scriptGlobalBits) &&
      self->unitFlag != 0) {
    BtlCameraFocusUnit(1300.0f, 200.0f, 40.0f, BtlGetCameraTask(), self, 0, 0, NULL);
  }
}
