// bdc 0x0888b790 UiHpGaugeSetAllAlpha
#include "bdc.h"

/* Sets the alpha of every gauge in the HP-gauge draw group (`g_uiHpGaugeGroup`): `fade = alpha`,
   `alpha` field = source anchor alpha (`anchor[3]`) × alpha. Used to fade the gauges out at the end
   of a battle (`BtlHudPhaseBattleEnd`). */

void UiHpGaugeSetAllAlpha(float alpha)

{
  UiHpGauge *g;

  for (g = (UiHpGauge *)g_uiHpGaugeGroup.head; g != (UiHpGauge *)0x0;
       g = (UiHpGauge *)g->base.next) {
    g->fade = alpha;
    g->alpha = ((UiHpGauge *)g->source)->anchor[3] * alpha;
  }
  return;
}
