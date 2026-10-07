// bdc 0x088fd94c GameQuestCamModeBaseStaticInit
#include "bdc.h"

/* Static initialiser (C++ constructor table entry `0x08af5cd8`) of the quest camera mode base
   unit (`GameQuestCamModeBaseCtor`): stores the ten float4 axis constants at
   `0x08abfe40..0x08abfedf` in order axis X, axis Y, axis Z, zero, +Y, -Y, -X, +X, -Z, +Z (every
   w = 0). No matrix is written. */
void GameQuestCamModeBaseStaticInit(void)
{
  g_gameQuestCamModeBaseAxisConsts.axisX.x = 1.0f;
  g_gameQuestCamModeBaseAxisConsts.axisX.y = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.axisX.z = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.axisX.w = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.axisY.x = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.axisY.y = 1.0f;
  g_gameQuestCamModeBaseAxisConsts.axisY.z = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.axisY.w = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.axisZ.x = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.axisZ.y = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.axisZ.z = 1.0f;
  g_gameQuestCamModeBaseAxisConsts.axisZ.w = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.zero.x = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.zero.y = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.zero.z = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.zero.w = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.up.x = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.up.y = 1.0f;
  g_gameQuestCamModeBaseAxisConsts.up.z = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.up.w = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.down.x = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.down.z = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.down.y = -1.0f;
  g_gameQuestCamModeBaseAxisConsts.down.w = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.negX.x = -1.0f;
  g_gameQuestCamModeBaseAxisConsts.negX.y = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.negX.z = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.negX.w = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.posX.x = 1.0f;
  g_gameQuestCamModeBaseAxisConsts.posX.y = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.posX.z = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.posX.w = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.negZ.x = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.negZ.y = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.negZ.z = -1.0f;
  g_gameQuestCamModeBaseAxisConsts.negZ.w = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.posZ.x = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.posZ.y = 0.0f;
  g_gameQuestCamModeBaseAxisConsts.posZ.z = 1.0f;
  g_gameQuestCamModeBaseAxisConsts.posZ.w = 0.0f;
}
