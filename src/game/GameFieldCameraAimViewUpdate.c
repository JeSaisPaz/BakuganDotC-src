// bdc 0x088ccd0c GameFieldCameraAimViewUpdate
#include "bdc.h"

/* Per-frame step of the field camera's throw aim helper (`cam+0x400`, a
   `GameFieldCameraSpringCtor` holder plus eye/look-at targets `+0x10`/`+0x20` and aim direction
   `+0x30`): unless the player is in state 8 (then it just hides the aim sprite `player+0x4d0`, flag
   bit 0 of `+0xd0`), pitches (`GameFieldCameraAimViewPitch`), computes targets
   (`GameFieldCameraAimViewComputeTargets`), turns the player
   (`GameFieldCameraAimViewTurnPlayer`), tests walls (`GameFieldCameraAimViewCollide`) and
   springs eye/look-at into `outEye`/`outLook` (`GameFieldCameraSpringStep`, damping 0.8). */

typedef struct {
  u8 pad[0xd0];
  u32 flags;
} GameFieldAimSprite;

typedef struct {
  u8 pad[0x4d0];
  GameFieldAimSprite *sprite;
} GameFieldAimPlayer;

void GameFieldCameraAimViewUpdate(void *aim, float *outEye, float *outLook)

{
  Actor *player;
  GameFieldAimSprite *sprite;
  float targetEye[4] __attribute__((aligned(16)));
  float targetLook[4] __attribute__((aligned(16)));

  player = (Actor *)ActorFindPlayer();
  if (player->state == 8) {
    sprite = ((GameFieldAimPlayer *)player)->sprite;
    if (sprite != NULL) {
      sprite->flags = sprite->flags & ~1u;
    }
  } else {
    GameFieldCameraAimViewPitch(aim);
    GameFieldCameraAimViewComputeTargets(aim);
    GameFieldCameraAimViewTurnPlayer(aim);
    GameFieldCameraAimViewCollide(aim, targetEye, targetLook);
    GameFieldCameraSpringStep(0.8f, aim, outEye, outLook, targetEye, targetLook);
  }
}
