// bdc 0x088a0108 GameGimmickCorePointUpdateLaunchScale
#include "bdc.h"

/* During the first hop of a launched core point (`+0x2d0 == 0`) scales the model scale vector
   `+0x2a0` by the eased hop progress (`w*2t(1-t)+t^2`, weight `g_gimmickLaunchScaleWeight`). */

extern float g_gimmickLaunchScaleWeight;

void GameGimmickCorePointUpdateLaunchScale(GameGimmickCorePoint *obj)
{
  float t;
  float f;

  if (obj->hopCount == 0) {
    t = (float)obj->hopFrame / (float)obj->hopFrames;
    f = g_gimmickLaunchScaleWeight * 2.0f * t * (1.0f - t) + t * t;
    /* vscl.t + sv.q: lane w gets a stale VFPU value on the PSP; left untouched here */
    obj->scale.x = obj->scale.x * f;
    obj->scale.y = obj->scale.y * f;
    obj->scale.z = obj->scale.z * f;
  }
}
