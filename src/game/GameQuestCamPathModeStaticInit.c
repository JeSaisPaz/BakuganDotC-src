// bdc 0x088f7f08 GameQuestCamPathModeStaticInit
#include "bdc.h"

/* Static initialiser (C++ constructor table entry `0x08af5c90`) of the quest path-camera unit:
   stores the ten float4 axis constants at `0x08abf890..0x08abf92f` in order axis X, axis Y, axis
   Z, zero, +Y, -Y, -X, +X, -Z, +Z (every w = 0). No matrix is written. */
void GameQuestCamPathModeStaticInit(void)
{
  g_gameQuestCamPathModeAxisConsts.axisX.x = 1.0f;
  g_gameQuestCamPathModeAxisConsts.axisX.y = 0.0f;
  g_gameQuestCamPathModeAxisConsts.axisX.z = 0.0f;
  g_gameQuestCamPathModeAxisConsts.axisX.w = 0.0f;
  g_gameQuestCamPathModeAxisConsts.axisY.x = 0.0f;
  g_gameQuestCamPathModeAxisConsts.axisY.y = 1.0f;
  g_gameQuestCamPathModeAxisConsts.axisY.z = 0.0f;
  g_gameQuestCamPathModeAxisConsts.axisY.w = 0.0f;
  g_gameQuestCamPathModeAxisConsts.axisZ.x = 0.0f;
  g_gameQuestCamPathModeAxisConsts.axisZ.y = 0.0f;
  g_gameQuestCamPathModeAxisConsts.axisZ.z = 1.0f;
  g_gameQuestCamPathModeAxisConsts.axisZ.w = 0.0f;
  g_gameQuestCamPathModeAxisConsts.zero.x = 0.0f;
  g_gameQuestCamPathModeAxisConsts.zero.y = 0.0f;
  g_gameQuestCamPathModeAxisConsts.zero.z = 0.0f;
  g_gameQuestCamPathModeAxisConsts.zero.w = 0.0f;
  g_gameQuestCamPathModeAxisConsts.up.x = 0.0f;
  g_gameQuestCamPathModeAxisConsts.up.y = 1.0f;
  g_gameQuestCamPathModeAxisConsts.up.z = 0.0f;
  g_gameQuestCamPathModeAxisConsts.up.w = 0.0f;
  g_gameQuestCamPathModeAxisConsts.down.x = 0.0f;
  g_gameQuestCamPathModeAxisConsts.down.z = 0.0f;
  g_gameQuestCamPathModeAxisConsts.down.y = -1.0f;
  g_gameQuestCamPathModeAxisConsts.down.w = 0.0f;
  g_gameQuestCamPathModeAxisConsts.negX.x = -1.0f;
  g_gameQuestCamPathModeAxisConsts.negX.y = 0.0f;
  g_gameQuestCamPathModeAxisConsts.negX.z = 0.0f;
  g_gameQuestCamPathModeAxisConsts.negX.w = 0.0f;
  g_gameQuestCamPathModeAxisConsts.posX.x = 1.0f;
  g_gameQuestCamPathModeAxisConsts.posX.y = 0.0f;
  g_gameQuestCamPathModeAxisConsts.posX.z = 0.0f;
  g_gameQuestCamPathModeAxisConsts.posX.w = 0.0f;
  g_gameQuestCamPathModeAxisConsts.negZ.x = 0.0f;
  g_gameQuestCamPathModeAxisConsts.negZ.y = 0.0f;
  g_gameQuestCamPathModeAxisConsts.negZ.z = -1.0f;
  g_gameQuestCamPathModeAxisConsts.negZ.w = 0.0f;
  g_gameQuestCamPathModeAxisConsts.posZ.x = 0.0f;
  g_gameQuestCamPathModeAxisConsts.posZ.y = 0.0f;
  g_gameQuestCamPathModeAxisConsts.posZ.z = 1.0f;
  g_gameQuestCamPathModeAxisConsts.posZ.w = 0.0f;
}
