// bdc 0x08a2bfd8 ActorStageObjLandmarkHidesHpGauge
#include "bdc.h"

/* Landmark override of stage-object vtable slot 18 (`+0x94`): returns 1 when the landmark's helper
   object `+0x32c` exists, so `UiHpGaugeIsSourceHidden` hides its HP gauge. */

int ActorStageObjLandmarkHidesHpGauge(ActorStageObjLandmark *self)

{
  return (uint)(self->helper != (void *)0x0);
}

