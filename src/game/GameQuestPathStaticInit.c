// bdc 0x088f65ec GameQuestPathStaticInit
#include "bdc.h"

/* Static initialiser (C++ constructor table entry `0x08af5c80`) of the quest path unit: stores
   the ten float4 axis constants at `0x08abf750..0x08abf7ef` in order axis X, axis Y, axis Z,
   zero, +Y, -Y, -X, +X, -Z, +Z (every w = 0). No matrix is written. */
void GameQuestPathStaticInit(void)
{
  g_gameQuestPathAxisConsts.axisX.x = 1.0f;
  g_gameQuestPathAxisConsts.axisX.y = 0.0f;
  g_gameQuestPathAxisConsts.axisX.z = 0.0f;
  g_gameQuestPathAxisConsts.axisX.w = 0.0f;
  g_gameQuestPathAxisConsts.axisY.x = 0.0f;
  g_gameQuestPathAxisConsts.axisY.y = 1.0f;
  g_gameQuestPathAxisConsts.axisY.z = 0.0f;
  g_gameQuestPathAxisConsts.axisY.w = 0.0f;
  g_gameQuestPathAxisConsts.axisZ.x = 0.0f;
  g_gameQuestPathAxisConsts.axisZ.y = 0.0f;
  g_gameQuestPathAxisConsts.axisZ.z = 1.0f;
  g_gameQuestPathAxisConsts.axisZ.w = 0.0f;
  g_gameQuestPathAxisConsts.zero.x = 0.0f;
  g_gameQuestPathAxisConsts.zero.y = 0.0f;
  g_gameQuestPathAxisConsts.zero.z = 0.0f;
  g_gameQuestPathAxisConsts.zero.w = 0.0f;
  g_gameQuestPathAxisConsts.up.x = 0.0f;
  g_gameQuestPathAxisConsts.up.y = 1.0f;
  g_gameQuestPathAxisConsts.up.z = 0.0f;
  g_gameQuestPathAxisConsts.up.w = 0.0f;
  g_gameQuestPathAxisConsts.down.x = 0.0f;
  g_gameQuestPathAxisConsts.down.z = 0.0f;
  g_gameQuestPathAxisConsts.down.y = -1.0f;
  g_gameQuestPathAxisConsts.down.w = 0.0f;
  g_gameQuestPathAxisConsts.negX.x = -1.0f;
  g_gameQuestPathAxisConsts.negX.y = 0.0f;
  g_gameQuestPathAxisConsts.negX.z = 0.0f;
  g_gameQuestPathAxisConsts.negX.w = 0.0f;
  g_gameQuestPathAxisConsts.posX.x = 1.0f;
  g_gameQuestPathAxisConsts.posX.y = 0.0f;
  g_gameQuestPathAxisConsts.posX.z = 0.0f;
  g_gameQuestPathAxisConsts.posX.w = 0.0f;
  g_gameQuestPathAxisConsts.negZ.x = 0.0f;
  g_gameQuestPathAxisConsts.negZ.y = 0.0f;
  g_gameQuestPathAxisConsts.negZ.z = -1.0f;
  g_gameQuestPathAxisConsts.negZ.w = 0.0f;
  g_gameQuestPathAxisConsts.posZ.x = 0.0f;
  g_gameQuestPathAxisConsts.posZ.y = 0.0f;
  g_gameQuestPathAxisConsts.posZ.z = 1.0f;
  g_gameQuestPathAxisConsts.posZ.w = 0.0f;
}
