// bdc 0x088d8464 GameGimmickCameraCooldownState
#include "bdc.h"

/* Cooldown state of the surveillance camera gimmick (`GameGimmickCameraCtor`, vtables
   `0x08af325c`/`0x08af3304`) (step `+0x180`): waits for the player's release flag `+0x355`, then
   the timer `+0x258`, then returns to state 0. */

void GameGimmickCameraCooldownState(GameGimmickCamera *obj)
{
  s32 step = obj->step;

  if (step < 1) {
    if (step >= 0) {
      Actor *player = (Actor *)ActorFindPlayer();
      s32 released = 1;

      if (player != (Actor *)0 && player->alerted == 0) {
        released = 0;
      }
      if (released) {
        obj->cooldown = 0;
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
