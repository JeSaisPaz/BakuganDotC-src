// bdc 0x088c6ae8 GameFieldStaticInit
#include "bdc.h"

/* Static initialiser (entry 16 of `g_cxxCtorTable`, the C++ constructor table at `0x08af5bbc`) of
   the field task unit: stores the angle constants `g_gameFieldAngleConsts` (16 halfwords),
   `g_gameFieldAngleWord900` (a word) and `g_gameFieldAngle904`, then the ten float4 axis
   constants `g_gameFieldAxisConsts` (axis X, Y, Z, zero, +Y, -Y, -X, +X, -Z, +Z; every w = 0). */

void GameFieldStaticInit(void)
{
  g_gameFieldAngleConsts[0] = 0x71c;
  g_gameFieldAngleConsts[1] = 0x1555;
  g_gameFieldAngleConsts[2] = 0x222;
  g_gameFieldAngleConsts[3] = 0x222;
  g_gameFieldAngleConsts[4] = 0x2000;
  g_gameFieldAngleConsts[5] = 0xb6;
  g_gameFieldAngleConsts[6] = 0x2000;
  g_gameFieldAngleConsts[7] = 0xb6;
  g_gameFieldAngleConsts[8] = 0x2000;
  g_gameFieldAngleConsts[9] = 0xb6;
  g_gameFieldAngleConsts[10] = 0x2000;
  g_gameFieldAngleConsts[11] = 0x2d8;
  g_gameFieldAngleConsts[12] = 0x2000;
  g_gameFieldAngleConsts[13] = 0xb6;
  g_gameFieldAngleConsts[14] = 0x4000;
  g_gameFieldAngleConsts[15] = 0x38e3;
  g_gameFieldAngleWord900 = 0x1555;
  g_gameFieldAngle904 = 0x2aaa;
  g_gameFieldAxisConsts.axisX.x = 1.0f;
  g_gameFieldAxisConsts.axisX.y = 0.0f;
  g_gameFieldAxisConsts.axisX.z = 0.0f;
  g_gameFieldAxisConsts.axisX.w = 0.0f;
  g_gameFieldAxisConsts.axisY.x = 0.0f;
  g_gameFieldAxisConsts.axisY.y = 1.0f;
  g_gameFieldAxisConsts.axisY.z = 0.0f;
  g_gameFieldAxisConsts.axisY.w = 0.0f;
  g_gameFieldAxisConsts.axisZ.x = 0.0f;
  g_gameFieldAxisConsts.axisZ.y = 0.0f;
  g_gameFieldAxisConsts.axisZ.z = 1.0f;
  g_gameFieldAxisConsts.axisZ.w = 0.0f;
  g_gameFieldAxisConsts.zero.x = 0.0f;
  g_gameFieldAxisConsts.zero.y = 0.0f;
  g_gameFieldAxisConsts.zero.z = 0.0f;
  g_gameFieldAxisConsts.zero.w = 0.0f;
  g_gameFieldAxisConsts.up.x = 0.0f;
  g_gameFieldAxisConsts.up.y = 1.0f;
  g_gameFieldAxisConsts.up.z = 0.0f;
  g_gameFieldAxisConsts.up.w = 0.0f;
  g_gameFieldAxisConsts.down.x = 0.0f;
  g_gameFieldAxisConsts.down.z = 0.0f;
  g_gameFieldAxisConsts.down.y = -1.0f;
  g_gameFieldAxisConsts.down.w = 0.0f;
  g_gameFieldAxisConsts.negX.x = -1.0f;
  g_gameFieldAxisConsts.negX.y = 0.0f;
  g_gameFieldAxisConsts.negX.z = 0.0f;
  g_gameFieldAxisConsts.negX.w = 0.0f;
  g_gameFieldAxisConsts.posX.x = 1.0f;
  g_gameFieldAxisConsts.posX.y = 0.0f;
  g_gameFieldAxisConsts.posX.z = 0.0f;
  g_gameFieldAxisConsts.posX.w = 0.0f;
  g_gameFieldAxisConsts.negZ.x = 0.0f;
  g_gameFieldAxisConsts.negZ.y = 0.0f;
  g_gameFieldAxisConsts.negZ.z = -1.0f;
  g_gameFieldAxisConsts.negZ.w = 0.0f;
  g_gameFieldAxisConsts.posZ.x = 0.0f;
  g_gameFieldAxisConsts.posZ.y = 0.0f;
  g_gameFieldAxisConsts.posZ.z = 1.0f;
  g_gameFieldAxisConsts.posZ.w = 0.0f;
}
