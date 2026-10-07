// bdc 0x088db278 GameGimmickTouchSpotUpdate
#include "bdc.h"

/* Per-frame update of the touch-spot gimmick (vtable `0x08af3684` slot 7): runs state 0 from the
   member-pointer table `0x08a96c68`, then `GameGimmickTouchSpotCheckContact`. */

void GameGimmickTouchSpotUpdate(GameGimmickTouchSpot *gimmick)
{
  s32 state = gimmick->base.state;

  if (state >= 0 && state == 0) {
    const VtblEntry *entry = &g_gimmickTouchSpotStateTable[state];
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
  GameGimmickTouchSpotCheckContact(gimmick);
}
