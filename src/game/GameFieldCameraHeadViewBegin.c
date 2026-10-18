// bdc 0x088c90bc GameFieldCameraHeadViewBegin
#include "bdc.h"

/* Starts the field camera's head view helper (`GameFieldCameraHeadView`, `cam+0x3d0`): takes the
   player's (`ActorFindPlayer`) `"Bip01_Head"` node position, sets `targetLook` halfway between it
   and the object position `obj[8..11]`, and `targetEye` = `targetLook` + 30 * (player Y rotation
   applied to (-0.6, 0, 0.4)) raised by 5; both `w` lanes are 0. Then resets the spring to the
   current eye/look-at `obj[0..7]` (`GameFieldCameraSpringReset`). */

void GameFieldCameraHeadViewBegin(void *view, float *obj)
{
  GameFieldCameraHeadView *hv = (GameFieldCameraHeadView *)view;
  float *v = obj;
  GfxModel *player;
  ScePspFVector4 head BDC_ALIGN16;
  float heading;
  float c;
  float s;
  float offX;
  float offY;
  float offZ;
  float tx;
  float tz;

  player = (GfxModel *)ActorFindPlayer();
  GfxModelGetNodeWorldPos(player, &head, "Bip01_Head");
  /* targetLook = (head - objPos) * 0.5 + objPos */
  hv->targetLook[0] = VfAdd(VfMul(VfSub(head.x, v[8]), 0.5f), v[8]);
  hv->targetLook[1] = VfAdd(VfMul(VfSub(head.y, v[9]), 0.5f), v[9]);
  hv->targetLook[2] = VfAdd(VfMul(VfSub(head.z, v[10]), 0.5f), v[10]);
  hv->targetLook[3] = 0.0f;
  /* view matrix = inverse (transpose) of the Y rotation by the player's heading; applied to
     (-0.6, 0, 0.4, 0) */
  heading = player->rot[1];
  c = VfCosQuarter(VfMul(heading, 0.636619747f));
  s = VfSinQuarter(VfMul(heading, 0.636619747f));
  /* inverse translation: vtfm3.t of the rotation rows by the zero translation (lane y all-constant: 0) */
  tx = VfDot2(c, -s, 0.0f, 0.0f);
  tz = VfDot2(s, c, 0.0f, 0.0f);
  /* vtfm4.q: (-0.6, 0, 0.4, 0) times the inverse rows (c,0,s,0) (0,1,0,0) (-s,0,c,0) (-tx,-0,-tz,1) */
  offX = VfDot3(-0.6f, 0.4f, 0.0f, c, -s, -tx);
  offY = 0.0f;
  offZ = VfDot3(-0.6f, 0.4f, 0.0f, s, c, -tz);
  hv->targetEye[0] = VfAdd(hv->targetLook[0], VfMul(offX, 30.0f));
  hv->targetEye[1] = VfAdd(hv->targetLook[1], VfMul(offY, 30.0f));
  hv->targetEye[2] = VfAdd(hv->targetLook[2], VfMul(offZ, 30.0f));
  hv->targetEye[3] = 0.0f;
  hv->targetEye[1] = hv->targetEye[1] + 5.0f;
  GameFieldCameraSpringReset((void **)view, obj);
}
