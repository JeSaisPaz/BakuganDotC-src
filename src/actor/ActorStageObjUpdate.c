// bdc 0x088ac654 ActorStageObjUpdate
#include "bdc.h"

/* Shared per-frame update of the stage objects (scenery props with HP, `ActorStageObjBaseCtor`):
   unless `ActorStageObjRecordSkipsFade` holds, runs `ActorStageObjUpdateFade` with radius
   `diagonal * 5` and far distance 4000 (2000 when `SaveGetProfileFlag0` is set), translucency allowed
   unless `fade < 1`, and stores the visible flag (always 1 when skipped); sets the draw alpha
   `ambient[3] = baseAlpha * fade`; then `ActorStageObjUpdateLightAlpha`, the state handler
   `g_actorStageObjStateFns``[state]` for states 0..11 (`ActorStageObjUpdateLightPositions` when the
   state is > 0 afterwards), and finally the break virtual (vtable slot 11) when `removeRequest` is set,
   or else `ActorStageObjRebuildMatrix``(self, 0)`. */

void ActorStageObjUpdate(ActorStageObjBase *self)

{
  float farDist;
  float alpha;
  int hidden;
  s32 state;
  const MemberFnPtr *member;
  const VtblEntry *brk;
  u8 *obj;
  void *fn;

  if (ActorStageObjRecordSkipsFade(self) == 0) {
    farDist = 4000.0f;
    if (SaveGetProfileFlag0() != 0) {
      farDist = 2000.0f;
    }
    hidden = ActorStageObjUpdateFade(self->diagonal * 5.0f, farDist, self, self, &self->baseAlpha,
                                     (char *)&self->fadeState, !(self->fade < 1.0f));
    alpha = self->fade;
    self->visible = hidden == 0;
    alpha = self->baseAlpha * alpha;
  }
  else {
    alpha = self->fade;
    self->visible = 1;
    alpha = self->baseAlpha * alpha;
  }
  self->base.ambient[3] = alpha;
  ActorStageObjUpdateLightAlpha(self);
  state = self->state;
  if ((-1 < state) && ((u32)state < 0xc)) {
    member = &g_actorStageObjStateFns[state];
    obj = (u8 *)self + member->delta;
    fn = member->pfn;
    if (member->index != 0) {
      const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)member->pfn);
      const VtblEntry *entry = &vtbl[member->index];

      fn = entry->fn;
      obj += entry->delta;
    }
    ((void (*)(void *))fn)(obj);
    if (0 < self->state) {
      ActorStageObjUpdateLightPositions(self);
    }
  }
  if (self->removeRequest != 0) {
    brk = &((const VtblEntry *)self->base.base.vtable)[11];
    ((void (*)(void *))brk->fn)((u8 *)self + brk->delta);
    return;
  }
  ActorStageObjRebuildMatrix(self, 0);
  return;
}
