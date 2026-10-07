// bdc 0x088c90bc GameFieldCameraHeadViewBegin
#include "bdc.h"

/* Starts the field camera's head view helper (`GameFieldCameraHeadView`, `cam+0x3d0`): takes the
   player's (`ActorFindPlayer`) `"Bip01_Head"` node position, sets `targetLook` halfway between it
   and the object position `obj[8..11]`, and `targetEye` = `targetLook` + 30 * (player Y rotation
   applied to (-0.6, 0, 0.4)) raised by 5; both `w` lanes are 0. Then resets the spring to the
   current eye/look-at `obj[0..7]` (`GameFieldCameraSpringReset`). */

void GameFieldCameraHeadViewBegin(void *view, void *obj)
{
  GameFieldCameraHeadView *hv = (GameFieldCameraHeadView *)view;
  float *v = (float *)obj;
  GfxModel *player;
  ScePspFVector4 head BDC_ALIGN16;
  float heading;
  float c;
  float s;
  float offX;
  float offY;
  float offZ;

  player = (GfxModel *)ActorFindPlayer();
  GfxModelGetNodeWorldPos(player, &head, "Bip01_Head");
  /* targetLook = (head - objPos) * 0.5 + objPos */
  hv->targetLook[0] = (head.x - v[8]) * 0.5f + v[8];
  hv->targetLook[1] = (head.y - v[9]) * 0.5f + v[9];
  hv->targetLook[2] = (head.z - v[10]) * 0.5f + v[10];
  hv->targetLook[3] = 0.0f;
  /* view matrix = inverse (transpose) of the Y rotation by the player's heading; applied to
     (-0.6, 0, 0.4, 0) */
  heading = player->rot[1];
  c = __builtin_cosf(heading);
  s = __builtin_sinf(heading);
  offX = c * -0.6f + -s * 0.4f;
  offY = 0.0f;
  offZ = s * -0.6f + c * 0.4f;
  hv->targetEye[0] = hv->targetLook[0] + offX * 30.0f;
  hv->targetEye[1] = hv->targetLook[1] + offY * 30.0f;
  hv->targetEye[2] = hv->targetLook[2] + offZ * 30.0f;
  hv->targetEye[3] = 0.0f;
  hv->targetEye[1] = hv->targetEye[1] + 5.0f;
  GameFieldCameraSpringReset((void **)view, (float *)obj);
}
