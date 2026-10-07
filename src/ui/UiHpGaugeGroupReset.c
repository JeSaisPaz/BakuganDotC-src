// bdc 0x0888b778 UiHpGaugeGroupReset
#include "bdc.h"

/* Empties the HP-gauge draw group (`g_uiHpGaugeGroup`): head, tail and count = 0.
   Called when a battle loads (`BtlMainPhaseLoad`). */

void UiHpGaugeGroupReset(void)

{
  g_uiHpGaugeGroup.tail = 0;
  g_uiHpGaugeGroup.head = 0;
  g_uiHpGaugeGroup.count = 0;
  return;
}
