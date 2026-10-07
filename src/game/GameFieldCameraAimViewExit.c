// bdc 0x088ccddc GameFieldCameraAimViewExit
#include "bdc.h"

/* Exit of the field camera's throw aim helper (`cam+0x400`, a `GameFieldCameraSpringCtor` holder
   plus eye/look-at targets `+0x10`/`+0x20` and aim direction `+0x30`): restores the player's alpha
   `+0x6c` to 1. */

void GameFieldCameraAimViewExit(void *aim)

{
  Actor *player;

  player = (Actor *)ActorFindPlayer();
  player->base.ambient[3] = 1.0f;
  return;
}
