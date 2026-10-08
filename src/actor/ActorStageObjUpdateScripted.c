// bdc 0x088a8dac ActorStageObjUpdateScripted
#include "bdc.h"

/* Update method of the script-created HP stage object (`ActorStageObjCtor`, vtable `0x08af2864`
   slot 7): runs `ActorStageObjUpdate`; once destroyed (`+0x281`), a single time records it as the
   last activated object (`g_gameLastActivatedObject`, see `GameClearLastActivatedObject`;
   `ActorStageObjCountStandingTargets` checks a pending event), flags the companion unit defeated,
   stops its effects (`GfxEffectStopOwned`) and marks its layout spawn record `+0x154` done
   (`+0x3a`); then runs state `+0x32c` 0 of table `0x08a84068` (`ActorStageObjScriptedState00`).
    */

void ActorStageObjUpdateScripted(ActorStageObjScripted *self)
{
  ActorStageObjUpdate(&self->base);
  if ((self->base).dead != '\0' && self->deathNotified == '\0') {
    ActorStageObjRecord *rec;

    g_gameLastActivatedObject = self;
    if (ActorStageObjCountStandingTargets() == 0) {
      g_gameLastActivatedObject = self;
    }
    if (self->unit != NULL) {
      self->unit->combat.dead = 1;
    }
    GfxEffectStopOwned(g_btlUnitEffectMgr, -1, self);
    rec = (ActorStageObjRecord *)(self->base).record;
    if (rec != NULL) {
      rec->doneFlags[0] = 1;
    }
    self->deathNotified = 1;
  }
  if (self->state >= 0 && self->state < 1) {
    const VtblEntry *entry = &g_actorStageObjScriptedStateTable[self->state];
    u8 *obj = (u8 *)self + entry->delta;
    void (*fn)(void *) = (void (*)(void *))entry->fn;

    if (entry->pad != 0) {
      const VtblEntry *v = (const VtblEntry *)*(void **)(obj + (intptr_t)fn) + entry->pad;
      fn = (void (*)(void *))v->fn;
      obj += v->delta;
    }
    fn(obj);
  }
}
