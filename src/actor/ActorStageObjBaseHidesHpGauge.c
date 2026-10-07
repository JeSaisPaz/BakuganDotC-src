// bdc 0x08a2bff4 ActorStageObjBaseHidesHpGauge
#include "bdc.h"

/* Base stage-object virtual slot 18 (`+0x94`): returns 0, i.e. the HP gauge of the object stays
   visible (`UiHpGaugeIsSourceHidden`); overridden by `ActorStageObjLandmarkHidesHpGauge`. */

int ActorStageObjBaseHidesHpGauge(ActorStageObjBase *self)

{
  return 0;
}

