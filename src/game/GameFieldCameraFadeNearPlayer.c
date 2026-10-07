// bdc 0x088bb4fc GameFieldCameraFadeNearPlayer
#include "bdc.h"

/* Fades the player actor (`ActorFindPlayer`) out when the camera eye `eye` is within 6.72 units
   of it horizontally: steps the player's model alpha `base.ambient[3]` (+0x6c) by 0.2 per frame
   towards 0 (close, clamped to 0 once at or below 0) or 1 (far, clamped to 1 once at or above 1). */

void GameFieldCameraFadeNearPlayer(float *eye)
{
  Actor *player;
  float dx;
  float dz;
  float dist;

  player = (Actor *)ActorFindPlayer();
  /* eye - player pos with the height lane zeroed, then its length */
  dx = eye[0] - player->base.pos[0];
  dz = eye[2] - player->base.pos[2];
  dist = __builtin_sqrtf(dx * dx + dz * dz);

  if (dist < 6.7200003f) {
    if (player->base.ambient[3] <= 0.0f) {
      player->base.ambient[3] = 0.0f;
    } else {
      player->base.ambient[3] = player->base.ambient[3] - 0.2f;
    }
  } else if (player->base.ambient[3] < 1.0f) {
    player->base.ambient[3] = player->base.ambient[3] + 0.2f;
  } else {
    player->base.ambient[3] = 1.0f;
  }
}
