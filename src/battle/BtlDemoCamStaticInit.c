// bdc 0x088fdaec BtlDemoCamStaticInit
#include "bdc.h"

/* Static initialiser (C++ constructor table entry `0x08af5ce0`) of the battle demo camera unit
   (`BtlDemoCamCtor`): fills `g_btlDemoCamAxisConsts` (`AxisVectorConsts`) with the unit
   axes X, Y, Z, the zero vector, then +Y, -Y, -X, +X, -Z and +Z (w = 0). */
void BtlDemoCamStaticInit(void)
{
  g_btlDemoCamAxisConsts.axisX.x = 1.0f;
  g_btlDemoCamAxisConsts.axisX.y = 0.0f;
  g_btlDemoCamAxisConsts.axisX.z = 0.0f;
  g_btlDemoCamAxisConsts.axisX.w = 0.0f;
  g_btlDemoCamAxisConsts.axisY.x = 0.0f;
  g_btlDemoCamAxisConsts.axisY.y = 1.0f;
  g_btlDemoCamAxisConsts.axisY.z = 0.0f;
  g_btlDemoCamAxisConsts.axisY.w = 0.0f;
  g_btlDemoCamAxisConsts.axisZ.x = 0.0f;
  g_btlDemoCamAxisConsts.axisZ.y = 0.0f;
  g_btlDemoCamAxisConsts.axisZ.z = 1.0f;
  g_btlDemoCamAxisConsts.axisZ.w = 0.0f;
  g_btlDemoCamAxisConsts.zero.x = 0.0f;
  g_btlDemoCamAxisConsts.zero.y = 0.0f;
  g_btlDemoCamAxisConsts.zero.z = 0.0f;
  g_btlDemoCamAxisConsts.zero.w = 0.0f;
  g_btlDemoCamAxisConsts.up.x = 0.0f;
  g_btlDemoCamAxisConsts.up.y = 1.0f;
  g_btlDemoCamAxisConsts.up.z = 0.0f;
  g_btlDemoCamAxisConsts.up.w = 0.0f;
  g_btlDemoCamAxisConsts.down.x = 0.0f;
  g_btlDemoCamAxisConsts.down.z = 0.0f;
  g_btlDemoCamAxisConsts.down.y = -1.0f;
  g_btlDemoCamAxisConsts.down.w = 0.0f;
  g_btlDemoCamAxisConsts.negX.x = -1.0f;
  g_btlDemoCamAxisConsts.negX.y = 0.0f;
  g_btlDemoCamAxisConsts.negX.z = 0.0f;
  g_btlDemoCamAxisConsts.negX.w = 0.0f;
  g_btlDemoCamAxisConsts.posX.x = 1.0f;
  g_btlDemoCamAxisConsts.posX.y = 0.0f;
  g_btlDemoCamAxisConsts.posX.z = 0.0f;
  g_btlDemoCamAxisConsts.posX.w = 0.0f;
  g_btlDemoCamAxisConsts.negZ.x = 0.0f;
  g_btlDemoCamAxisConsts.negZ.y = 0.0f;
  g_btlDemoCamAxisConsts.negZ.z = -1.0f;
  g_btlDemoCamAxisConsts.negZ.w = 0.0f;
  g_btlDemoCamAxisConsts.posZ.x = 0.0f;
  g_btlDemoCamAxisConsts.posZ.y = 0.0f;
  g_btlDemoCamAxisConsts.posZ.z = 1.0f;
  g_btlDemoCamAxisConsts.posZ.w = 0.0f;
}
