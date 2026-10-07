// bdc 0x088d698c GameGimmickIrSensorCooldownState
#include "bdc.h"

/* Cooldown state of the IR sensor gimmick (`GameGimmickIrSensorCtor`, vtables
   `0x08af303c`/`0x08af30e4`) (step `+0x180`): waits until the player is no longer held in state 9
   (or released, `+0x355`), counts 150 frames (`+0x1b0`), then returns to state 0. */

void GameGimmickIrSensorCooldownState(GameGimmickIrSensor *obj)
{
  int step = obj->step;

  if (step < 1) {
    if (-1 < step) {
      bool release = true;
      Actor *player = ActorFindPlayer();

      if (player != NULL && player->state == 9 && player->alerted == 0) {
        release = false;
      }
      if (release) {
        obj->cooldown = 0x96;
        obj->step = 1;
      }
    }
  } else if (step < 2) {
    if (obj->cooldown < 1) {
      obj->step = 2;
    } else {
      obj->cooldown = obj->cooldown - 1;
    }
  } else if (step < 3) {
    obj->base.state = 0;
    obj->step = 0;
  }
}
