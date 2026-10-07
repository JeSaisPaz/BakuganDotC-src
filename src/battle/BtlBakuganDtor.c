// bdc 0x0885f290 BtlBakuganDtor
#include "bdc.h"

/* Destructor of the battle Bakugan (vtable `0x08af1fa4` slot 1; also chained from the subclass
   destructors `BtlTargetPointDtor`, `ActorCrystalDtor` and `BtlCpuUnitDtor`). Does nothing
   for NULL. Otherwise reinstalls `g_btlBakuganVtbl`; for unit kinds (`unk08`) 0xd, 0xa, 8 and
   0x14 releases the four `legs` sprites from their layers (`UiSpriteLayerRelease`); then
   releases, clearing each pointer: `shadow` (`BtlShadowDtor``(obj, 3)`), `input` (virtual
   destructor slot 1 with flags 3), `motionTable` (`MemFree` under `MemLock`), `collider0`
   and `collider1` (virtual destructors), the model sound (`GfxModelReleaseSound`), `hpGauge`
   and `auxNode` (virtual destructors, vtable at `+0x20`), `weapon`
   (`BtlSwordBlurDelete``(obj, 3)`), `wingModel` (virtual destructor, vtable at `+0x14`) and
   `stats` (`BtlStatsDestroy`, pointer left as is). Finally destroys the embedded combat state
   (`BtlCombatDtor``(&combat, 2)`), runs the model base destructor (`GfxModelDtor``(self, 0)`)
   and frees the object when `flags & 1`. */

/* GCC 2.x virtual destructor call: slot 1 of the vtable, `this` adjusted by the entry delta. */
#define BTL_VDELETE(obj, vtbl)                                                       \
  do {                                                                               \
    const VtblEntry *dtor_ = &((const VtblEntry *)(vtbl))[1];                        \
    ((void (*)(void *, u32))dtor_->fn)((u8 *)(obj) + dtor_->delta, 3);               \
  } while (0)

void BtlBakuganDtor(BtlBakugan *self, u32 flags)
{
  u32 kind;
  int i;

  if (self == NULL) {
    return;
  }
  self->base.base.vtable = g_btlBakuganVtbl;
  kind = self->base.base.unk08;
  if (kind == 0xd || kind == 10 || kind == 8 || kind == 0x14) {
    for (i = 0; i < 4; i++) {
      GfxEffect *leg = self->legs[i];

      if (leg != NULL) {
        UiSpriteLayerRelease(leg->mgr, leg);
      }
    }
  }
  if (self->shadow != NULL) {
    BtlShadowDtor((BtlShadow *)self->shadow, 3);
    self->shadow = NULL;
  }
  if (self->input != NULL) {
    BTL_VDELETE(self->input, self->input->vtbl);
    self->input = NULL;
  }
  if (self->motionTable != NULL) {
    s16 *table = self->motionTable;

    MemLock();
    MemFree(table, NULL, 0);
    MemUnlock();
    self->motionTable = NULL;
  }
  if (self->collider0 != NULL) {
    BTL_VDELETE(self->collider0, self->collider0->node.vtable);
    self->collider0 = NULL;
  }
  if (self->collider1 != NULL) {
    BTL_VDELETE(self->collider1, self->collider1->node.vtable);
    self->collider1 = NULL;
  }
  GfxModelReleaseSound(&self->base);
  if (self->hpGauge != NULL) {
    BTL_VDELETE(self->hpGauge, ((CoreNode *)self->hpGauge)->vtable);
    self->hpGauge = NULL;
  }
  if (self->auxNode != NULL) {
    BTL_VDELETE(self->auxNode, self->auxNode->vtable);
    self->auxNode = NULL;
  }
  if (self->weapon != NULL) {
    BtlSwordBlurDelete(self->weapon, 3);
    self->weapon = NULL;
  }
  if (self->wingModel != NULL) {
    BTL_VDELETE(self->wingModel, ((CoreObject *)self->wingModel)->vtable);
    self->wingModel = NULL;
  }
  if (self->stats != NULL) {
    BtlStatsDestroy(self->stats);
  }
  BtlCombatDtor(&self->combat, 2);
  GfxModelDtor(&self->base, 0);
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(self, NULL, 0);
    MemUnlock();
  }
}
