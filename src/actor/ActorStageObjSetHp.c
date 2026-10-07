// bdc 0x088ac90c ActorStageObjSetHp
#include "bdc.h"

/* Sets a stage object's hit points: truncates `hp` to an integer and stores it in both `+0x204`
   (maximum) and `+0x200` (current), then, when the object already owns its HP gauge (`+0x290`),
   snaps the gauge's displayed values with `UiHpGaugeSetValues`. */

void ActorStageObjSetHp(float hp, ActorStageObjBase *self)

{
  self->maxHp = (int)hp;
  self->hp = (int)hp;
  if (self->hpGauge != (UiHpGauge *)0x0) {
    UiHpGaugeSetValues(hp,self->hpGauge);
  }
  return;
}

