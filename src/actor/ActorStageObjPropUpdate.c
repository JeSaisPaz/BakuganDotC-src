// bdc 0x088b0bb4 ActorStageObjPropUpdate
#include "bdc.h"

/* Update method of the knock-over prop (vtable `0x08af2ae4` slot 7, `ActorStageObjPropCtor`):
   distance fade (translucency allowed while idle), draw alpha, then the state `+0x350` from table
   `g_actorStageObjPropStateFns`: 0 `ActorStageObjPropState00Inactive`, 1
   `ActorStageObjPropState01Idle`, 2 `ActorStageObjPropState02Knocked`, 3
   `ActorStageObjPropState03Topple`; the pipe unit (kind 0x1b) also updates its lights
   (`ActorStageObjUpdateLightAlpha`, `ActorStageObjUpdateLightPositions`). */

void ActorStageObjPropUpdate(ActorStageObjProp *self)
{
  const MemberFnPtr *member;
  u8 *obj;
  void *fn;
  int visible;

  visible = ActorStageObjUpdateFade(self->base.diagonal * 5.0f, 4000.0f, self, &self->base,
                                    &self->base.baseAlpha, (char *)&self->base.fadeState,
                                    self->step == 0);
  self->base.visible = visible == 0;
  self->base.base.ambient[3] = self->base.baseAlpha * self->base.fade;
  if (self->state >= 0 && (unsigned)self->state < 4) {
    member = &g_actorStageObjPropStateFns[self->state];
    obj = (u8 *)self + member->delta;
    fn = member->pfn;
    if (member->index != 0) {
      const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)member->pfn);
      const VtblEntry *entry = &vtbl[member->index];

      fn = entry->fn;
      obj += entry->delta;
    }
    ((void (*)(void *))fn)(obj);
    if (self->base.kind == 0x1b) {
      ActorStageObjUpdateLightAlpha(&self->base);
      if (self->state > 0) {
        ActorStageObjUpdateLightPositions(&self->base);
      }
    }
  }
}
