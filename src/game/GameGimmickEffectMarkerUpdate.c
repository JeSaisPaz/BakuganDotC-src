// bdc 0x088da998 GameGimmickEffectMarkerUpdate
#include "bdc.h"

/* Per-frame update of the effect-marker gimmick (vtable `0x08af3524` slot 7): runs state 0 from the
   member-pointer table `g_gameGimmickEffectMarkerStateTable`, `GameGimmickEffectMarkerSyncPlayer`
   and the base `GameGimmickUpdate`. */

void GameGimmickEffectMarkerUpdate(GameGimmickEffectMarker *gimmick)
{
  int state = gimmick->base.state;

  if (state >= 0 && state < 1) {
    const VtblEntry *entry = &g_gameGimmickEffectMarkerStateTable[state];
    u8 *obj = (u8 *)gimmick + entry->delta;
    void (*fn)(void *) = (void (*)(void *))entry->fn;

    if (entry->pad != 0) {
      const VtblEntry *v = (const VtblEntry *)*(void **)(obj + (intptr_t)entry->fn) + entry->pad;
      fn = (void (*)(void *))v->fn;
      obj += v->delta;
    }
    fn(obj);
  }
  GameGimmickEffectMarkerSyncPlayer(gimmick);
  GameGimmickUpdate(&gimmick->base);
}
