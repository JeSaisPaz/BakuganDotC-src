// bdc 0x088fca28 GameQuestCamTargetStaticInit
#include "bdc.h"

/* Static initialiser (C++ constructor table entry `0x08af5cd0`) of the quest camera target/apply
   unit (`GameQuestCamTargetCtor`, `GameQuestCamResolveCollision`): stores the ten float4
   axis constants at `0x08abfda0..0x08abfe3f` in order axis X, axis Y, axis Z, zero, +Y, -Y, -X,
   +X, -Z, +Z (every w = 0). No matrix is written. */
void GameQuestCamTargetStaticInit(void)
{
  g_gameQuestCamTargetAxisConsts.axisX.x = 1.0f;
  g_gameQuestCamTargetAxisConsts.axisX.y = 0.0f;
  g_gameQuestCamTargetAxisConsts.axisX.z = 0.0f;
  g_gameQuestCamTargetAxisConsts.axisX.w = 0.0f;
  g_gameQuestCamTargetAxisConsts.axisY.x = 0.0f;
  g_gameQuestCamTargetAxisConsts.axisY.y = 1.0f;
  g_gameQuestCamTargetAxisConsts.axisY.z = 0.0f;
  g_gameQuestCamTargetAxisConsts.axisY.w = 0.0f;
  g_gameQuestCamTargetAxisConsts.axisZ.x = 0.0f;
  g_gameQuestCamTargetAxisConsts.axisZ.y = 0.0f;
  g_gameQuestCamTargetAxisConsts.axisZ.z = 1.0f;
  g_gameQuestCamTargetAxisConsts.axisZ.w = 0.0f;
  g_gameQuestCamTargetAxisConsts.zero.x = 0.0f;
  g_gameQuestCamTargetAxisConsts.zero.y = 0.0f;
  g_gameQuestCamTargetAxisConsts.zero.z = 0.0f;
  g_gameQuestCamTargetAxisConsts.zero.w = 0.0f;
  g_gameQuestCamTargetAxisConsts.up.x = 0.0f;
  g_gameQuestCamTargetAxisConsts.up.y = 1.0f;
  g_gameQuestCamTargetAxisConsts.up.z = 0.0f;
  g_gameQuestCamTargetAxisConsts.up.w = 0.0f;
  g_gameQuestCamTargetAxisConsts.down.x = 0.0f;
  g_gameQuestCamTargetAxisConsts.down.z = 0.0f;
  g_gameQuestCamTargetAxisConsts.down.y = -1.0f;
  g_gameQuestCamTargetAxisConsts.down.w = 0.0f;
  g_gameQuestCamTargetAxisConsts.negX.x = -1.0f;
  g_gameQuestCamTargetAxisConsts.negX.y = 0.0f;
  g_gameQuestCamTargetAxisConsts.negX.z = 0.0f;
  g_gameQuestCamTargetAxisConsts.negX.w = 0.0f;
  g_gameQuestCamTargetAxisConsts.posX.x = 1.0f;
  g_gameQuestCamTargetAxisConsts.posX.y = 0.0f;
  g_gameQuestCamTargetAxisConsts.posX.z = 0.0f;
  g_gameQuestCamTargetAxisConsts.posX.w = 0.0f;
  g_gameQuestCamTargetAxisConsts.negZ.x = 0.0f;
  g_gameQuestCamTargetAxisConsts.negZ.y = 0.0f;
  g_gameQuestCamTargetAxisConsts.negZ.z = -1.0f;
  g_gameQuestCamTargetAxisConsts.negZ.w = 0.0f;
  g_gameQuestCamTargetAxisConsts.posZ.x = 0.0f;
  g_gameQuestCamTargetAxisConsts.posZ.y = 0.0f;
  g_gameQuestCamTargetAxisConsts.posZ.z = 1.0f;
  g_gameQuestCamTargetAxisConsts.posZ.w = 0.0f;
}
