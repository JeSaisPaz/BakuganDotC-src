// bdc 0x088fc1f4 GameQuestCamTableStaticInit
#include "bdc.h"

/* Static initialiser (C++ constructor table entry `0x08af5cc0`) of the quest camera table unit
   (`GameQuestCamTableCtor`): stores the ten float4 axis constants at `0x08abfc60..0x08abfcff`
   in order axis X, axis Y, axis Z, zero, +Y, -Y, -X, +X, -Z, +Z (every w = 0). No matrix is
   written. */
void GameQuestCamTableStaticInit(void)
{
  g_gameQuestCamTableAxisConsts.axisX.x = 1.0f;
  g_gameQuestCamTableAxisConsts.axisX.y = 0.0f;
  g_gameQuestCamTableAxisConsts.axisX.z = 0.0f;
  g_gameQuestCamTableAxisConsts.axisX.w = 0.0f;
  g_gameQuestCamTableAxisConsts.axisY.x = 0.0f;
  g_gameQuestCamTableAxisConsts.axisY.y = 1.0f;
  g_gameQuestCamTableAxisConsts.axisY.z = 0.0f;
  g_gameQuestCamTableAxisConsts.axisY.w = 0.0f;
  g_gameQuestCamTableAxisConsts.axisZ.x = 0.0f;
  g_gameQuestCamTableAxisConsts.axisZ.y = 0.0f;
  g_gameQuestCamTableAxisConsts.axisZ.z = 1.0f;
  g_gameQuestCamTableAxisConsts.axisZ.w = 0.0f;
  g_gameQuestCamTableAxisConsts.zero.x = 0.0f;
  g_gameQuestCamTableAxisConsts.zero.y = 0.0f;
  g_gameQuestCamTableAxisConsts.zero.z = 0.0f;
  g_gameQuestCamTableAxisConsts.zero.w = 0.0f;
  g_gameQuestCamTableAxisConsts.up.x = 0.0f;
  g_gameQuestCamTableAxisConsts.up.y = 1.0f;
  g_gameQuestCamTableAxisConsts.up.z = 0.0f;
  g_gameQuestCamTableAxisConsts.up.w = 0.0f;
  g_gameQuestCamTableAxisConsts.down.x = 0.0f;
  g_gameQuestCamTableAxisConsts.down.z = 0.0f;
  g_gameQuestCamTableAxisConsts.down.y = -1.0f;
  g_gameQuestCamTableAxisConsts.down.w = 0.0f;
  g_gameQuestCamTableAxisConsts.negX.x = -1.0f;
  g_gameQuestCamTableAxisConsts.negX.y = 0.0f;
  g_gameQuestCamTableAxisConsts.negX.z = 0.0f;
  g_gameQuestCamTableAxisConsts.negX.w = 0.0f;
  g_gameQuestCamTableAxisConsts.posX.x = 1.0f;
  g_gameQuestCamTableAxisConsts.posX.y = 0.0f;
  g_gameQuestCamTableAxisConsts.posX.z = 0.0f;
  g_gameQuestCamTableAxisConsts.posX.w = 0.0f;
  g_gameQuestCamTableAxisConsts.negZ.x = 0.0f;
  g_gameQuestCamTableAxisConsts.negZ.y = 0.0f;
  g_gameQuestCamTableAxisConsts.negZ.z = -1.0f;
  g_gameQuestCamTableAxisConsts.negZ.w = 0.0f;
  g_gameQuestCamTableAxisConsts.posZ.x = 0.0f;
  g_gameQuestCamTableAxisConsts.posZ.y = 0.0f;
  g_gameQuestCamTableAxisConsts.posZ.z = 1.0f;
  g_gameQuestCamTableAxisConsts.posZ.w = 0.0f;
}
