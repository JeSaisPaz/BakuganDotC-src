// bdc 0x088bd61c GameFieldCameraExitOrbit
#include "bdc.h"

/* Mode 7 exit handler of the field camera (`GameFieldCameraCtor`): restores the player's alpha
   `+0x6c` to 1. */

void GameFieldCameraExitOrbit(GameFieldCamera *cam)

{
  Actor *player;

  player = (Actor *)ActorFindPlayer();
  player->base.ambient[3] = 1.0f;
  return;
}
