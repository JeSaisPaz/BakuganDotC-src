// bdc 0x088d6a58 GameGimmickIrSensorAdvanceStepState
#include "bdc.h"

/* State 2 handler of the IR sensor gimmick (`GameGimmickIrSensorCtor`): advances the step
   `+0x180` from 0 to 1 and from 1 to 2, then leaves it (no other effect). */

void GameGimmickIrSensorAdvanceStepState(GameGimmickIrSensor *obj)

{
  if (obj->step > 0) {
    if (obj->step < 2) {
      obj->step = 2;
    }
  } else if (obj->step >= 0) {
    obj->step = 1;
  }
}
