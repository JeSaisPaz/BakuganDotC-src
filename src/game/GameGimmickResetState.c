// bdc 0x088d6744 GameGimmickResetState
#include "bdc.h"

/* Resets the state `+0x16c` and step `+0x180` of a gimmick: always when `force`, otherwise only
   when it is in state 1. */

void GameGimmickResetState(GameGimmickIrSensor *obj, bool force)

{
  if (!force && obj->base.state != 1) {
    return;
  }
  obj->base.state = 0;
  obj->step = 0;
}
