// bdc 0x088f688c GameQuestCamFixedModeStaticInit
#include "bdc.h"

/* Static initialiser (C++ constructor table entry `0x08af5c88`) of the quest fixed-camera unit:
   fills `g_gameQuestCamFixedModeAxisConsts` (`0x08abf7f0..0x08abf88f`) with the ten float4
   axis constants in order axis X, axis Y, axis Z, zero, +Y, -Y, -X, +X, -Z, +Z (every w = 0). No
   matrix is written. */
void GameQuestCamFixedModeStaticInit(void)
{
  g_gameQuestCamFixedModeAxisConsts.axisX.x = 1.0f;
  g_gameQuestCamFixedModeAxisConsts.axisX.y = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.axisX.z = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.axisX.w = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.axisY.x = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.axisY.y = 1.0f;
  g_gameQuestCamFixedModeAxisConsts.axisY.z = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.axisY.w = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.axisZ.x = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.axisZ.y = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.axisZ.z = 1.0f;
  g_gameQuestCamFixedModeAxisConsts.axisZ.w = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.zero.x = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.zero.y = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.zero.z = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.zero.w = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.up.x = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.up.y = 1.0f;
  g_gameQuestCamFixedModeAxisConsts.up.z = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.up.w = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.down.x = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.down.z = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.down.y = -1.0f;
  g_gameQuestCamFixedModeAxisConsts.down.w = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.negX.x = -1.0f;
  g_gameQuestCamFixedModeAxisConsts.negX.y = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.negX.z = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.negX.w = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.posX.x = 1.0f;
  g_gameQuestCamFixedModeAxisConsts.posX.y = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.posX.z = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.posX.w = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.negZ.x = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.negZ.y = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.negZ.z = -1.0f;
  g_gameQuestCamFixedModeAxisConsts.negZ.w = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.posZ.x = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.posZ.y = 0.0f;
  g_gameQuestCamFixedModeAxisConsts.posZ.z = 1.0f;
  g_gameQuestCamFixedModeAxisConsts.posZ.w = 0.0f;
}
