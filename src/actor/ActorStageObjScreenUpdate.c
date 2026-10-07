// bdc 0x088b0798 ActorStageObjScreenUpdate
#include "bdc.h"

/* Update method of the screen stage object (vtable `0x08af2a44` slot 7,
   `ActorStageObjScreenCtor`): distance fade (far 2500), motion update/apply
   (`GfxModelUpdateMotion`, `GfxModelApplyMotion`); while the battle task is in phase 6 it
   hides the screen (alpha 0, `ActorStageObjScreenMaterialAdditiveHidden` via `GfxModelForEachMaterial`),
   otherwise shows it with `ActorStageObjScreenMaterialAdditive`; then runs state 0 of
   `g_actorStageObjScreenStateFns` (the empty `ActorStageObjScreenState00Nop`). */

void ActorStageObjScreenUpdate(ActorStageObjScreen *self)

{
  u8 shown;
  int hidden;
  int state;
  BtlMain *main;
  const MemberFnPtr *member;
  u8 *obj;
  void *fn;

  hidden = ActorStageObjUpdateFade(self->base.diagonal * 5.0f, 2500.0f, self, &self->base,
                                   &self->base.baseAlpha, (char *)&self->base.fadeState, 0);
  self->base.visible = hidden == 0;
  GfxModelUpdateMotion(&self->base.base);
  GfxModelApplyMotion(&self->base.base);
  self->base.base.ambient[3] = self->base.baseAlpha * self->base.fade;
  if (BtlCameraTaskExists() != 0) {
    main = (BtlMain *)BtlGetCameraTask();
    shown = self->shown;
    if (main->phase == 6) {
      self->base.base.visible = 0;
      if (shown == 1) {
        self->base.fade = 0.0f;
        self->base.base.ambient[3] = 0.0f;
        GfxModelForEachMaterial(&self->base.base, ActorStageObjScreenMaterialAdditiveHidden, NULL);
        self->shown = 0;
      }
    }
    else if (shown == 0) {
      self->base.fade = 0.999f;
      GfxModelForEachMaterial(&self->base.base, ActorStageObjScreenMaterialAdditive, NULL);
      self->shown = 1;
    }
  }
  state = self->state;
  if ((-1 < state) && ((u32)state < 1)) {
    member = &g_actorStageObjScreenStateFns[state];
    obj = (u8 *)self + member->delta;
    fn = member->pfn;
    if (member->index != 0) {
      const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)member->pfn);
      const VtblEntry *entry = &vtbl[member->index];

      fn = entry->fn;
      obj += entry->delta;
    }
    ((void (*)(void *))fn)(obj);
  }
  return;
}
