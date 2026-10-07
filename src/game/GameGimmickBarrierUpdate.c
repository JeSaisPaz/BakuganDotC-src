// bdc 0x088d9d04 GameGimmickBarrierUpdate
#include "bdc.h"

/* Per-frame update of the barrier gimmick (vtable `0x08af33c4` slot 7): dispatches state `+0x16c`
   (0..1) through the member-pointer table `0x08a96c10`, then `GameGimmickBarrierPulse`. */

void GameGimmickBarrierUpdate(GameGimmickBarrier *gimmick)
{
  u32 state = (u32)gimmick->base.state;

  if (state < 2) {
    const VtblEntry *entry = &g_gimmickBarrierStateTable[state];
    u8 *self = (u8 *)gimmick + entry->delta;
    void *fn = entry->fn;

    if (entry->pad != 0) {
      const VtblEntry *vt = *(const VtblEntry **)(self + (intptr_t)fn);

      vt += entry->pad;
      fn = vt->fn;
      self += vt->delta;
    }
    ((void (*)(void *))fn)(self);
  }
  GameGimmickBarrierPulse(gimmick);
}
