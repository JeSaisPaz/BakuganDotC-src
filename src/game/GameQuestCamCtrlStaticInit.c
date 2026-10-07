// bdc 0x088fa704 GameQuestCamCtrlStaticInit
#include "bdc.h"

/* Static initialiser (C++ constructor table entry `0x08af5cb8`) of the quest camera controller
   unit (`GameQuestCamCtrlCtor`): stores the ten float4 axis constants at
   `0x08abfbb0..0x08abfc4f` in order axis X, axis Y, axis Z, zero, +Y, -Y, -X, +X, -Z, +Z (every
   w = 0); the +Y vector at `0x08abfbf0` is read by `GameQuestCamCtrlSwitchMode`. No matrix is
   written. */
void GameQuestCamCtrlStaticInit(void)
{
  g_gameQuestCamCtrlAxisConsts.axisX.x = 1.0f;
  g_gameQuestCamCtrlAxisConsts.axisX.y = 0.0f;
  g_gameQuestCamCtrlAxisConsts.axisX.z = 0.0f;
  g_gameQuestCamCtrlAxisConsts.axisX.w = 0.0f;
  g_gameQuestCamCtrlAxisConsts.axisY.x = 0.0f;
  g_gameQuestCamCtrlAxisConsts.axisY.y = 1.0f;
  g_gameQuestCamCtrlAxisConsts.axisY.z = 0.0f;
  g_gameQuestCamCtrlAxisConsts.axisY.w = 0.0f;
  g_gameQuestCamCtrlAxisConsts.axisZ.x = 0.0f;
  g_gameQuestCamCtrlAxisConsts.axisZ.y = 0.0f;
  g_gameQuestCamCtrlAxisConsts.axisZ.z = 1.0f;
  g_gameQuestCamCtrlAxisConsts.axisZ.w = 0.0f;
  g_gameQuestCamCtrlAxisConsts.zero.x = 0.0f;
  g_gameQuestCamCtrlAxisConsts.zero.y = 0.0f;
  g_gameQuestCamCtrlAxisConsts.zero.z = 0.0f;
  g_gameQuestCamCtrlAxisConsts.zero.w = 0.0f;
  g_gameQuestCamCtrlAxisConsts.up.x = 0.0f;
  g_gameQuestCamCtrlAxisConsts.up.y = 1.0f;
  g_gameQuestCamCtrlAxisConsts.up.z = 0.0f;
  g_gameQuestCamCtrlAxisConsts.up.w = 0.0f;
  g_gameQuestCamCtrlAxisConsts.down.x = 0.0f;
  g_gameQuestCamCtrlAxisConsts.down.z = 0.0f;
  g_gameQuestCamCtrlAxisConsts.down.y = -1.0f;
  g_gameQuestCamCtrlAxisConsts.down.w = 0.0f;
  g_gameQuestCamCtrlAxisConsts.negX.x = -1.0f;
  g_gameQuestCamCtrlAxisConsts.negX.y = 0.0f;
  g_gameQuestCamCtrlAxisConsts.negX.z = 0.0f;
  g_gameQuestCamCtrlAxisConsts.negX.w = 0.0f;
  g_gameQuestCamCtrlAxisConsts.posX.x = 1.0f;
  g_gameQuestCamCtrlAxisConsts.posX.y = 0.0f;
  g_gameQuestCamCtrlAxisConsts.posX.z = 0.0f;
  g_gameQuestCamCtrlAxisConsts.posX.w = 0.0f;
  g_gameQuestCamCtrlAxisConsts.negZ.x = 0.0f;
  g_gameQuestCamCtrlAxisConsts.negZ.y = 0.0f;
  g_gameQuestCamCtrlAxisConsts.negZ.z = -1.0f;
  g_gameQuestCamCtrlAxisConsts.negZ.w = 0.0f;
  g_gameQuestCamCtrlAxisConsts.posZ.x = 0.0f;
  g_gameQuestCamCtrlAxisConsts.posZ.y = 0.0f;
  g_gameQuestCamCtrlAxisConsts.posZ.z = 1.0f;
  g_gameQuestCamCtrlAxisConsts.posZ.w = 0.0f;
}
