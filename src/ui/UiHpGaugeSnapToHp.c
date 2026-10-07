// bdc 0x0888b880 UiHpGaugeSnapToHp
#include "bdc.h"

/* Sets the three displayed values `+0x74/+0x78/+0x7c` of the HUD hit-point gauge
   (`UiHpGaugeInit`, 0xa0 bytes) to the current HP (`UiHpGaugeGetHp`) and clears `+0x88`, so the
   bar jumps instead of animating. Used when a crystal regenerates (`ActorCrystalRegenerate`). */

void UiHpGaugeSnapToHp(UiHpGauge *self)

{
  float hp;
  
  hp = UiHpGaugeGetHp(self);
  self->hp = hp;
  self->hpTrail = hp;
  self->hpBeforeHit = hp;
  self->hitTimer = 0.0;
  return;
}

