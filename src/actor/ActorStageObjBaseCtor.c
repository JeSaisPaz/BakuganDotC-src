// bdc 0x088ab290 ActorStageObjBaseCtor
#include "bdc.h"

/* Base constructor shared by the stage-object classes (scenery props such as buildings, warehouses,
   cranes, containers, trees, landmarks). Builds the model with `GfxModelCtor``(obj, gmoName,
   0x200)` where `gmoName` is the first string of entry `kind` of the table at `0x08a8fe18`,
   installs the base vtable `0x08af2904` at `+0x14`, stores `kind` at `+0x218`, a category code from
   `ActorStageObjGetCategory(kind)` at `+0x208` and clears `+0x15c/+0x160`, then runs
   `ActorStageObjInitFields`, `ActorStageObjBaseInit(obj, pos)` (which also constructs
   `CollisionColliderCtor` colliders) and `ActorStageObjSaveRestPose`; when `GameStageIs0Or13()`
   is non-zero it also registers the object with `ActorStageObjCreateLights`. Returns `obj`. */

ActorStageObjBase *ActorStageObjBaseCtor(ActorStageObjBase *self, int kind, const float *pos)

{
  int category;
  s32 stage0Or13;
  
  GfxModelCtor(&self->base,g_actorStageObjModelTable[kind * 3],0x200);
  (self->base).base.vtable = &g_actorStageObjBaseVtbl;
  self->kind = kind;
  category = ActorStageObjGetCategory(kind);
  self->category = category;
  self->breakModel = (void *)0x0;
  self->remains = (void *)0x0;
  ActorStageObjInitFields(self);
  ActorStageObjBaseInit(self,(u32 *)pos);
  ActorStageObjSaveRestPose(self);
  stage0Or13 = GameStageIs0Or13();
  if (stage0Or13 != 0) {
    ActorStageObjCreateLights(self);
  }
  return self;
}

