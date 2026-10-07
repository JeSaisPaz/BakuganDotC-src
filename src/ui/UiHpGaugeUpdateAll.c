// bdc 0x0888b738 UiHpGaugeUpdateAll
#include "bdc.h"

/* Runs `UiHpGaugeUpdate` on every gauge in the HP-gauge draw group (list head ``g_uiHpGaugeGroup``, next
   pointer `+4`). Called by `BtlMainUpdateScene` and `BtlMainPhaseSceneOnly`. */

void UiHpGaugeUpdateAll(void)

{
  UiHpGauge *self;
  
  for (self = (UiHpGauge *)g_uiHpGaugeGroup.head; self != (UiHpGauge *)0x0; self = (UiHpGauge *)(self->base).next) {
    UiHpGaugeUpdate(self);
  }
  return;
}

