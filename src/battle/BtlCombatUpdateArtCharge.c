// bdc 0x08888d8c BtlCombatUpdateArtCharge
#include "bdc.h"

/* Per-frame special-art update of a `BtlCombatState`: runs `BtlCombatChargeArtInPinch` for
   every equipped slot (tier = art id % 4, C remainder); then, at the first equipped slot that is
   fully charged (not below 10000) while `g_btlBattleOver` is clear, window 0xb is active
   (`UiGetWindowActive`) and the battle main task is in phase 1 (`BtlGetCameraTask`),
   increments the ready counter `artReadyFrames` once. The window/camera calls are re-made for
   each equipped slot checked. */

void BtlCombatUpdateArtCharge(BtlCombatState *combat)
{
  s32 slot;

  for (slot = 0; slot < 3; slot++) {
    s32 artId = combat->artIds[slot];
    if (artId != -1) {
      BtlCombatChargeArtInPinch(combat, slot, artId % 4);
    }
  }
  for (slot = 0; slot < 3; slot++) {
    if (combat->artIds[slot] == -1) continue;
    if (g_btlBattleOver != 0) continue;
    if (UiGetWindowActive(0xb) == 0) continue;
    if (BtlCameraTaskExists() == 0) continue;
    if (((BtlMain *)BtlGetCameraTask())->phase != 1) continue;
    if (combat->artCharge[slot] < 10000.0f) continue;
    combat->artReadyFrames = combat->artReadyFrames + 1;
    return;
  }
}
