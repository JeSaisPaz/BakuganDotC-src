// bdc 0x088dba5c GameGimmickSwitchUpdate
#include "bdc.h"

/* Per-frame update of the switch gimmick (vtable `0x08af3734` slot 7): runs state 0 from the
   member-pointer table `0x08a96c70`; while visible (alpha `+0x6c` > 0) runs
   `GameGimmickSwitchUpdateGlow`, `GameGimmickSwitchCheckPressed` and
   `GameGimmickSwitchCheckPlayerFacing`; then advances the model pose (`GfxModelSetMotionSpeed(+0x190)`) and
   the base `GameGimmickUpdate`. */

void GameGimmickSwitchUpdate(GameGimmickSwitch *gimmick)
{
  u32 state = (u32)gimmick->base.state;

  if (state < 1) {
    const VtblEntry *entry = &g_gimmickSwitchStateTable[state];
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
  if (!(gimmick->base.base.ambient[3] <= 0.0f)) {
    GameGimmickSwitchUpdateGlow(gimmick);
    GameGimmickSwitchCheckPressed(gimmick);
    GameGimmickSwitchCheckPlayerFacing(gimmick);
  }
  GfxModelSetMotionSpeed(&gimmick->base.base, gimmick->pose);
  GameGimmickUpdate(&gimmick->base);
}
