// bdc 0x088daecc GameGimmickSolidUpdate
#include "bdc.h"

/* Per-frame update of the solid gimmick (vtable `0x08af35d4` slot 7): runs state 0 from the
   member-pointer table `0x08a96c5c`, the player-proximity test `GameGimmickSolidCheckPlayerNear`
   while it is visible (alpha `+0x6c` > 0) and the base `GameGimmickUpdate`. */

void GameGimmickSolidUpdate(GameGimmick *gimmick)
{
  u32 state = (u32)gimmick->state;

  if (state < 1) {
    const VtblEntry *entry = &g_gimmickSolidStateTable[state];
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
  if (!(gimmick->base.ambient[3] <= 0.0f)) {
    GameGimmickSolidCheckPlayerNear(gimmick);
  }
  GameGimmickUpdate(gimmick);
}
