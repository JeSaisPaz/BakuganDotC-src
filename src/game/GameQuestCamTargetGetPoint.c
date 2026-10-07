// bdc 0x08a2c808 GameQuestCamTargetGetPoint
#include "bdc.h"

/* Entry 7 (`+0x3c`) of the quest camera spring point vtable `0x08af6e58`, inherited by the mode
   base `0x08af458c` and the fixed mode `0x08af43d4`: copies the static zero vector `0x08abf820` to
   `out`. */

void GameQuestCamTargetGetPoint(GameQuestCamTarget *self, float *out)

{
  out[0] = g_gameQuestCamFixedModeAxisConsts.zero.x;
  out[1] = g_gameQuestCamFixedModeAxisConsts.zero.y;
  out[2] = g_gameQuestCamFixedModeAxisConsts.zero.z;
  out[3] = g_gameQuestCamFixedModeAxisConsts.zero.w;
}
