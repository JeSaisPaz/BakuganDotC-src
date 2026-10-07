// bdc 0x088f8f44 GameQuestCamLookSpringStaticInit
#include "bdc.h"

/* Static initialiser (C++ constructor table entry `0x08af5ca8`) of the quest look-spring unit:
   fills `g_gameQuestCamLookSpringAxisConsts` (`0x08abfa70..0x08abfb0f`) with the ten float4
   axis constants in order axis X, axis Y, axis Z, zero, +Y, -Y, -X, +X, -Z, +Z (every w = 0). No
   matrix is written. */
void GameQuestCamLookSpringStaticInit(void)
{
  g_gameQuestCamLookSpringAxisConsts.axisX.x = 1.0f;
  g_gameQuestCamLookSpringAxisConsts.axisX.y = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.axisX.z = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.axisX.w = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.axisY.x = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.axisY.y = 1.0f;
  g_gameQuestCamLookSpringAxisConsts.axisY.z = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.axisY.w = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.axisZ.x = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.axisZ.y = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.axisZ.z = 1.0f;
  g_gameQuestCamLookSpringAxisConsts.axisZ.w = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.zero.x = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.zero.y = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.zero.z = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.zero.w = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.up.x = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.up.y = 1.0f;
  g_gameQuestCamLookSpringAxisConsts.up.z = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.up.w = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.down.x = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.down.z = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.down.y = -1.0f;
  g_gameQuestCamLookSpringAxisConsts.down.w = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.negX.x = -1.0f;
  g_gameQuestCamLookSpringAxisConsts.negX.y = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.negX.z = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.negX.w = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.posX.x = 1.0f;
  g_gameQuestCamLookSpringAxisConsts.posX.y = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.posX.z = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.posX.w = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.negZ.x = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.negZ.y = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.negZ.z = -1.0f;
  g_gameQuestCamLookSpringAxisConsts.negZ.w = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.posZ.x = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.posZ.y = 0.0f;
  g_gameQuestCamLookSpringAxisConsts.posZ.z = 1.0f;
  g_gameQuestCamLookSpringAxisConsts.posZ.w = 0.0f;
}
