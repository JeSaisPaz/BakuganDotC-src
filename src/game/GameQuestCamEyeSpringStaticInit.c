// bdc 0x088f8b00 GameQuestCamEyeSpringStaticInit
#include "bdc.h"

/* Static initialiser (C++ constructor table entry `0x08af5ca0`) of the quest eye-spring unit:
   stores the ten float4 axis constants at `0x08abf9d0..0x08abfa6f` in order axis X, axis Y, axis
   Z, zero, +Y, -Y, -X, +X, -Z, +Z (every w = 0). No matrix is written. */
void GameQuestCamEyeSpringStaticInit(void)
{
  g_gameQuestCamEyeSpringAxisConsts.axisX.x = 1.0f;
  g_gameQuestCamEyeSpringAxisConsts.axisX.y = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.axisX.z = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.axisX.w = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.axisY.x = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.axisY.y = 1.0f;
  g_gameQuestCamEyeSpringAxisConsts.axisY.z = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.axisY.w = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.axisZ.x = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.axisZ.y = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.axisZ.z = 1.0f;
  g_gameQuestCamEyeSpringAxisConsts.axisZ.w = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.zero.x = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.zero.y = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.zero.z = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.zero.w = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.up.x = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.up.y = 1.0f;
  g_gameQuestCamEyeSpringAxisConsts.up.z = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.up.w = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.down.x = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.down.z = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.down.y = -1.0f;
  g_gameQuestCamEyeSpringAxisConsts.down.w = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.negX.x = -1.0f;
  g_gameQuestCamEyeSpringAxisConsts.negX.y = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.negX.z = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.negX.w = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.posX.x = 1.0f;
  g_gameQuestCamEyeSpringAxisConsts.posX.y = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.posX.z = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.posX.w = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.negZ.x = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.negZ.y = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.negZ.z = -1.0f;
  g_gameQuestCamEyeSpringAxisConsts.negZ.w = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.posZ.x = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.posZ.y = 0.0f;
  g_gameQuestCamEyeSpringAxisConsts.posZ.z = 1.0f;
  g_gameQuestCamEyeSpringAxisConsts.posZ.w = 0.0f;
}
