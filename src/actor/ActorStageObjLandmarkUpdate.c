// bdc 0x088a20d4 ActorStageObjLandmarkUpdate
#include "bdc.h"

/* Update method of the landmark (vtable `0x08af2434` slot 7): dispatches state `+0x3ec` (0..1)
   through the member-pointer table `0x08a83d70`: `ActorStageObjLandmarkState00Active`,
   `ActorStageObjLandmarkState01Broken`. */

void ActorStageObjLandmarkUpdate(ActorStageObjLandmark *self)
{
  if (self->state >= 0 && self->state < 2) {
    const VtblEntry *entry = &g_actorStageObjLandmarkStateTable[self->state];
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
