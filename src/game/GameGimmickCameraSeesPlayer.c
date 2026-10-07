// bdc 0x088d8370 GameGimmickCameraSeesPlayer
#include "bdc.h"

/* Returns 1 when the player is within 15 units of the spot sprite position (`+0x250 → +0x60`) of
   the surveillance camera gimmick (`GameGimmickCameraCtor`, vtables `0x08af325c`/`0x08af3304`)
   and not hidden (`ActorPlayerIsStealthed` = 0); never while profile word 0x2f is positive. */

s32 GameGimmickCameraSeesPlayer(GameGimmickCamera *obj)
{
  s32 result = 0;
  ActorPlayer *player = ActorFindPlayer();

  if (SaveHasProfile()) {
    if ((s32)SaveProfileGetWord(SaveGetProfile(), 0x2f) > 0) {
      player = NULL;
    }
  }
  if (player != NULL && ActorPlayerIsStealthed(player) == 0 && obj->spotEffect != NULL) {
    const float *a = ((GfxModel *)obj->spotEffect)->ambient;
    const float *p = player->base.base.pos;
    float dx = a[0] - p[0];
    float dy = a[1] - p[1];
    float dz = a[2] - p[2];
    float dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);

    if (dist <= 15.0f) {
      result = 1;
    }
  }
  return result;
}
