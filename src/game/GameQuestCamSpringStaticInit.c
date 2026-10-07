// bdc 0x088fc628 GameQuestCamSpringStaticInit
#include "bdc.h"

/* Static initialiser (C++ constructor table entry `0x08af5cc8`) of the quest camera spring unit
   (`GameQuestCamSpringCtor`): stores the ten float4 axis constants at `0x08abfd00..0x08abfd9f`
   in order axis X, axis Y, axis Z, zero, +Y, -Y, -X, +X, -Z, +Z (every w = 0). No matrix is
   written. */
void GameQuestCamSpringStaticInit(void)
{
  g_gameQuestCamSpringAxisConsts.axisX.x = 1.0f;
  g_gameQuestCamSpringAxisConsts.axisX.y = 0.0f;
  g_gameQuestCamSpringAxisConsts.axisX.z = 0.0f;
  g_gameQuestCamSpringAxisConsts.axisX.w = 0.0f;
  g_gameQuestCamSpringAxisConsts.axisY.x = 0.0f;
  g_gameQuestCamSpringAxisConsts.axisY.y = 1.0f;
  g_gameQuestCamSpringAxisConsts.axisY.z = 0.0f;
  g_gameQuestCamSpringAxisConsts.axisY.w = 0.0f;
  g_gameQuestCamSpringAxisConsts.axisZ.x = 0.0f;
  g_gameQuestCamSpringAxisConsts.axisZ.y = 0.0f;
  g_gameQuestCamSpringAxisConsts.axisZ.z = 1.0f;
  g_gameQuestCamSpringAxisConsts.axisZ.w = 0.0f;
  g_gameQuestCamSpringAxisConsts.zero.x = 0.0f;
  g_gameQuestCamSpringAxisConsts.zero.y = 0.0f;
  g_gameQuestCamSpringAxisConsts.zero.z = 0.0f;
  g_gameQuestCamSpringAxisConsts.zero.w = 0.0f;
  g_gameQuestCamSpringAxisConsts.up.x = 0.0f;
  g_gameQuestCamSpringAxisConsts.up.y = 1.0f;
  g_gameQuestCamSpringAxisConsts.up.z = 0.0f;
  g_gameQuestCamSpringAxisConsts.up.w = 0.0f;
  g_gameQuestCamSpringAxisConsts.down.x = 0.0f;
  g_gameQuestCamSpringAxisConsts.down.z = 0.0f;
  g_gameQuestCamSpringAxisConsts.down.y = -1.0f;
  g_gameQuestCamSpringAxisConsts.down.w = 0.0f;
  g_gameQuestCamSpringAxisConsts.negX.x = -1.0f;
  g_gameQuestCamSpringAxisConsts.negX.y = 0.0f;
  g_gameQuestCamSpringAxisConsts.negX.z = 0.0f;
  g_gameQuestCamSpringAxisConsts.negX.w = 0.0f;
  g_gameQuestCamSpringAxisConsts.posX.x = 1.0f;
  g_gameQuestCamSpringAxisConsts.posX.y = 0.0f;
  g_gameQuestCamSpringAxisConsts.posX.z = 0.0f;
  g_gameQuestCamSpringAxisConsts.posX.w = 0.0f;
  g_gameQuestCamSpringAxisConsts.negZ.x = 0.0f;
  g_gameQuestCamSpringAxisConsts.negZ.y = 0.0f;
  g_gameQuestCamSpringAxisConsts.negZ.z = -1.0f;
  g_gameQuestCamSpringAxisConsts.negZ.w = 0.0f;
  g_gameQuestCamSpringAxisConsts.posZ.x = 0.0f;
  g_gameQuestCamSpringAxisConsts.posZ.y = 0.0f;
  g_gameQuestCamSpringAxisConsts.posZ.z = 1.0f;
  g_gameQuestCamSpringAxisConsts.posZ.w = 0.0f;
}
