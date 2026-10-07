// bdc 0x0888b7d0 UiHpGaugeSetValues
#include "bdc.h"

/* Snaps all four displayed values of a HUD HP gauge to `value` (`+0x70`, `+0x74`, `+0x78` and
   `+0x7c`), i.e. max HP and the three current-HP display values (see `UiHpGaugeBind`) jump
   together without animation. Called right after a unit's max HP changes (`BtlCombatFillHp`,
   `BtlCombatSetLevel`, `ActorStageObjSetHp`). */

void UiHpGaugeSetValues(float value, UiHpGauge *self)

{
  self->hp = value;
  self->hpTrail = value;
  self->hpBeforeHit = value;
  self->maxHp = value;
  return;
}

