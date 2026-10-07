// bdc 0x088c8a68 GameFieldCameraMode9Exit
#include "bdc.h"

/* Exit of the field camera's mode-9 helper (`cam+0x3c0`, pointer to a 0x80-byte spring state with a
   probe at `+0x70`): restores the player's alpha `+0x6c` to 1. */

void GameFieldCameraMode9Exit(void **holder)

{
  Actor *player;

  player = (Actor *)ActorFindPlayer();
  player->base.ambient[3] = 1.0f;
  return;
}
