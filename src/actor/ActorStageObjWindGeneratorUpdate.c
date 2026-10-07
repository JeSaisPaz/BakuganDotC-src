// bdc 0x088a6640 ActorStageObjWindGeneratorUpdate
#include "bdc.h"

/* Update method of the wind generator (vtable `0x08af2714` slot 7,
   `ActorStageObjWindGeneratorCtor`): distance fade (`ActorStageObjUpdateFade`), draw alpha,
   model animation step (`GfxModelUpdateAndApplyMotion`) and, in state `+0x32c` 0, the single
   state handler of table `0x08a83f38` (`ActorStageObjWindGeneratorState00`). */

void ActorStageObjWindGeneratorUpdate(ActorStageObjWindGenerator *self)
{
  int hidden;

  hidden = ActorStageObjUpdateFade((self->base).diagonal * 5.0f, 4000.0f, self, &self->base,
                                   &(self->base).baseAlpha, (char *)&(self->base).fadeState,
                                   (self->base).step == 0);
  (self->base).visible = hidden == 0;
  (self->base).base.ambient[3] = (self->base).baseAlpha * (self->base).fade;
  GfxModelUpdateAndApplyMotion((GfxModel *)self);
  if (self->state >= 0 && self->state < 1) {
    const VtblEntry *entry = &g_actorStageObjWindGeneratorStateTable[self->state];
    u8 *obj = (u8 *)self + entry->delta;
    void (*fn)(void *) = (void (*)(void *))entry->fn;

    if (entry->pad != 0) {
      const VtblEntry *v = (const VtblEntry *)*(void **)(obj + (int)(intptr_t)fn) + entry->pad;
      fn = (void (*)(void *))v->fn;
      obj += v->delta;
    }
    fn(obj);
  }
}
