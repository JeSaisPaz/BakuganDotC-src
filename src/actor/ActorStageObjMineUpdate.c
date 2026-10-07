// bdc 0x088a5d54 ActorStageObjMineUpdate
#include "bdc.h"

/* Update method of the mine stage object (vtable `0x08af2674` slot 7, `ActorStageObjMineCtor`):
   on the first call saves the model colour into `tint` (`+0x250`), sets emissive `+0x240` to white,
   derives the mine type `+0x328` (`recordArg - 13`) and runs the material setup virtual `+0x54`
   (`ActorStageObjMineSetupMaterials`); marks its layout spawn record `+0x154` done once destroyed,
   runs the distance fade (`ActorStageObjUpdateFade`, translucency allowed only in state 0). For
   floating mines (kind 0xb3) it spawns the fuse aura effect 0x15b once while visible (alpha > 0)
   and no demo runs with the camera task alive, else stops it (`GfxEffectStopOwned`). Steps the
   UV scrolls, hides itself and stops the aura while a demo task (0x65/0x67) exists, then runs the
   state `+0x32c` from `g_actorStageObjMineStateFns`: `ActorStageObjMineState00Armed`,
   `ActorStageObjMineState01Triggered`. */

void ActorStageObjMineUpdate(ActorStageObjMine *self)

{
  const VtblEntry *setup;
  const MemberFnPtr *member;
  GfxEffect *effect;
  u8 *obj;
  void *fn;

  if (self->initialized == 0) {
    /* lv.q/sv.q quad copy color -> tint */
    self->base.tint[0] = self->base.base.color[0];
    self->base.tint[1] = self->base.base.color[1];
    self->base.tint[2] = self->base.base.color[2];
    self->base.tint[3] = self->base.base.color[3];
    self->base.emissive[0] = 1.0f;
    self->base.emissive[1] = 1.0f;
    self->base.emissive[2] = 1.0f;
    self->base.emissive[3] = 1.0f;
    self->mineType = self->base.recordArg - 13;
    setup = &((const VtblEntry *)self->base.base.base.vtable)[10];
    ((void (*)(void *))setup->fn)((u8 *)self + setup->delta);
    self->initialized = 1;
  }
  if (self->base.dead != 0 && self->recordMarked == 0) {
    if (self->base.record != NULL) {
      ((ActorStageObjRecord *)self->base.record)->doneFlags[0] = 1;
    }
    self->recordMarked = 1;
  }
  self->base.visible =
      ActorStageObjUpdateFade(self->base.diagonal * 5.0f, 4000.0f, self, &self->base,
                              &self->base.baseAlpha, (char *)&self->base.fadeState,
                              self->state == 0) == 0;
  self->base.base.ambient[3] = self->base.baseAlpha * self->base.fade;
  if (self->base.kind == 0xb3) {
    if (!(self->base.base.ambient[3] <= 0.0f) && BtlCameraTaskExists() != 0 &&
        (BtlGetCameraTask(), !BtlIsDemoRunning())) {
      if (self->fuseEffect == 0) {
        self->fuseEffect = 1;
        effect = (GfxEffect *)GfxEffectSpawn(g_btlUnitEffectMgr, 0x15b, self->base.base.pos);
        effect->ownerBakugan = self;
        if (self != NULL) {
          effect->ownerId = self->base.base.base.id;
        }
      }
    } else if (self->fuseEffect != 0) {
      self->fuseEffect = 0;
      GfxEffectStopOwned(g_btlUnitEffectMgr, -1, self);
    }
  }
  ActorStageObjStepUvScrolls(&self->base);
  if (CoreTaskExists(0x65) != 0 || CoreTaskExists(0x67) != 0) {
    self->base.base.visible = 0;
    if (self->fuseEffect != 0) {
      self->fuseEffect = 0;
      GfxEffectStopOwned(g_btlUnitEffectMgr, -1, self);
    }
  }
  if (self->state >= 0 && (u32)self->state < 2) {
    member = &g_actorStageObjMineStateFns[self->state];
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
}
