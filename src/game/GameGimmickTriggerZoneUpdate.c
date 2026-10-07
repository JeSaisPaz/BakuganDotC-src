// bdc 0x088da4cc GameGimmickTriggerZoneUpdate
#include "bdc.h"

/* Per-frame update of the trigger-zone gimmick (vtable `0x08af3474` slot 7): runs state 0 from the
   member-pointer table `0x08a96c48`, the base `GameGimmickUpdate` once the zone has been drawn
   (`+0x261`), and the player test `GameGimmickTriggerZoneCheckPlayer` while it is visible
   (`+0x164` clear). */

void GameGimmickTriggerZoneUpdate(GameGimmickTriggerZone *gimmick)
{
  u32 state = (u32)gimmick->base.state;

  if (state < 1) {
    const VtblEntry *entry = &g_gimmickTriggerZoneStateTable[state];
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
  if (gimmick->drawn != 0) {
    GameGimmickUpdate(&gimmick->base);
  }
  if (gimmick->base.hidden == 0) {
    GameGimmickTriggerZoneCheckPlayer(gimmick);
  }
}
