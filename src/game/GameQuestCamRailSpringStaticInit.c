// bdc 0x088f9388 GameQuestCamRailSpringStaticInit
#include "bdc.h"

/* Static initialiser (registered in the C++ constructor table at `0x08af5c68..`) of the quest
   rail-spring unit: stores the ten float4 axis constants of
   `g_gameQuestCamRailSpringAxisConsts` (`0x08abfb10..0x08abfbaf`) in order axis X, axis Y,
   axis Z, zero, +Y, -Y, -X, +X, -Z, +Z (every w = 0). */
void GameQuestCamRailSpringStaticInit(void)
{
  g_gameQuestCamRailSpringAxisConsts.axisX.x = 1.0f;
  g_gameQuestCamRailSpringAxisConsts.axisX.y = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.axisX.z = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.axisX.w = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.axisY.x = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.axisY.y = 1.0f;
  g_gameQuestCamRailSpringAxisConsts.axisY.z = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.axisY.w = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.axisZ.x = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.axisZ.y = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.axisZ.z = 1.0f;
  g_gameQuestCamRailSpringAxisConsts.axisZ.w = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.zero.x = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.zero.y = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.zero.z = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.zero.w = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.up.x = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.up.y = 1.0f;
  g_gameQuestCamRailSpringAxisConsts.up.z = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.up.w = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.down.x = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.down.y = -1.0f;
  g_gameQuestCamRailSpringAxisConsts.down.z = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.down.w = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.negX.x = -1.0f;
  g_gameQuestCamRailSpringAxisConsts.negX.y = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.negX.z = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.negX.w = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.posX.x = 1.0f;
  g_gameQuestCamRailSpringAxisConsts.posX.y = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.posX.z = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.posX.w = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.negZ.x = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.negZ.y = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.negZ.z = -1.0f;
  g_gameQuestCamRailSpringAxisConsts.negZ.w = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.posZ.x = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.posZ.y = 0.0f;
  g_gameQuestCamRailSpringAxisConsts.posZ.z = 1.0f;
  g_gameQuestCamRailSpringAxisConsts.posZ.w = 0.0f;
}
