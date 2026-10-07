// bdc 0x088f8778 GameQuestCamPointModeStaticInit
#include "bdc.h"

/* Static initialiser (C++ constructor table entry `0x08af5c98`) of the quest point-camera unit:
   fills `g_gameQuestCamPointModeAxisConsts` (`0x08abf930..0x08abf9cf`) with the ten float4
   axis constants in order axis X, axis Y, axis Z, zero, +Y, -Y, -X, +X, -Z, +Z (every w = 0). No
   matrix is written. */
void GameQuestCamPointModeStaticInit(void)
{
  g_gameQuestCamPointModeAxisConsts.axisX.x = 1.0f;
  g_gameQuestCamPointModeAxisConsts.axisX.y = 0.0f;
  g_gameQuestCamPointModeAxisConsts.axisX.z = 0.0f;
  g_gameQuestCamPointModeAxisConsts.axisX.w = 0.0f;
  g_gameQuestCamPointModeAxisConsts.axisY.x = 0.0f;
  g_gameQuestCamPointModeAxisConsts.axisY.y = 1.0f;
  g_gameQuestCamPointModeAxisConsts.axisY.z = 0.0f;
  g_gameQuestCamPointModeAxisConsts.axisY.w = 0.0f;
  g_gameQuestCamPointModeAxisConsts.axisZ.x = 0.0f;
  g_gameQuestCamPointModeAxisConsts.axisZ.y = 0.0f;
  g_gameQuestCamPointModeAxisConsts.axisZ.z = 1.0f;
  g_gameQuestCamPointModeAxisConsts.axisZ.w = 0.0f;
  g_gameQuestCamPointModeAxisConsts.zero.x = 0.0f;
  g_gameQuestCamPointModeAxisConsts.zero.y = 0.0f;
  g_gameQuestCamPointModeAxisConsts.zero.z = 0.0f;
  g_gameQuestCamPointModeAxisConsts.zero.w = 0.0f;
  g_gameQuestCamPointModeAxisConsts.up.x = 0.0f;
  g_gameQuestCamPointModeAxisConsts.up.y = 1.0f;
  g_gameQuestCamPointModeAxisConsts.up.z = 0.0f;
  g_gameQuestCamPointModeAxisConsts.up.w = 0.0f;
  g_gameQuestCamPointModeAxisConsts.down.x = 0.0f;
  g_gameQuestCamPointModeAxisConsts.down.z = 0.0f;
  g_gameQuestCamPointModeAxisConsts.down.y = -1.0f;
  g_gameQuestCamPointModeAxisConsts.down.w = 0.0f;
  g_gameQuestCamPointModeAxisConsts.negX.x = -1.0f;
  g_gameQuestCamPointModeAxisConsts.negX.y = 0.0f;
  g_gameQuestCamPointModeAxisConsts.negX.z = 0.0f;
  g_gameQuestCamPointModeAxisConsts.negX.w = 0.0f;
  g_gameQuestCamPointModeAxisConsts.posX.x = 1.0f;
  g_gameQuestCamPointModeAxisConsts.posX.y = 0.0f;
  g_gameQuestCamPointModeAxisConsts.posX.z = 0.0f;
  g_gameQuestCamPointModeAxisConsts.posX.w = 0.0f;
  g_gameQuestCamPointModeAxisConsts.negZ.x = 0.0f;
  g_gameQuestCamPointModeAxisConsts.negZ.y = 0.0f;
  g_gameQuestCamPointModeAxisConsts.negZ.z = -1.0f;
  g_gameQuestCamPointModeAxisConsts.negZ.w = 0.0f;
  g_gameQuestCamPointModeAxisConsts.posZ.x = 0.0f;
  g_gameQuestCamPointModeAxisConsts.posZ.y = 0.0f;
  g_gameQuestCamPointModeAxisConsts.posZ.z = 1.0f;
  g_gameQuestCamPointModeAxisConsts.posZ.w = 0.0f;
}
