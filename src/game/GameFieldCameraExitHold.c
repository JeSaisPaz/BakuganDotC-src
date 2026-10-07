// bdc 0x088bd644 GameFieldCameraExitHold
#include "bdc.h"

/* Mode 5 exit handler of the field camera (`GameFieldCameraCtor`): restores the player's alpha
   `+0x6c` to 1. */

void GameFieldCameraExitHold(GameFieldCamera *cam)

{
  Actor *player;

  player = (Actor *)ActorFindPlayer();
  player->base.ambient[3] = 1.0f;
  return;
}
