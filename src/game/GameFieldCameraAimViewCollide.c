// bdc 0x088ccc84 GameFieldCameraAimViewCollide
#include "bdc.h"

/* Runs a radius-4 wall test (`GameFieldCameraProbeCollideRadius`) between the player and the eye
   target of the field camera's throw aim helper (`cam+0x400`, a `GameFieldCameraSpringCtor`
   holder plus eye/look-at targets `+0x10`/`+0x20` and aim direction `+0x30`) into `outEye`, and
   copies the eye target `+0x10` to `outLook`. */

void GameFieldCameraAimViewCollide(void *aim, float *outEye, float *outLook)

{
  GameFieldCameraAimView *self = (GameFieldCameraAimView *)aim;
  Actor *player;
  u8 probe[8];

  player = (Actor *)ActorFindPlayer();
  GameFieldCameraProbeCtor(probe);
  GameFieldCameraProbeCollideRadius(4.0f,probe,outEye,outLook,self->lookAt,player->base.pos);
  outLook[0] = self->eye[0];
  outLook[1] = self->eye[1];
  outLook[2] = self->eye[2];
  outLook[3] = self->eye[3];
  GameFieldCameraProbeDtor(probe,2);
}
