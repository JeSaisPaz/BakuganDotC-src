// bdc 0x088bdc08 GameFieldCameraStaticInit
#include "bdc.h"

/* Static initialiser (entry 15 of `g_cxxCtorTable`, the C++ constructor table at `0x08af5bbc`) of
   the field camera unit: stores the ten float4 axis constants `g_gameFieldCameraAxisConsts`
   (axis X, Y, Z, zero, +Y, -Y, -X, +X, -Z, +Z; every w = 0), then the float constants at
   `0x08abd800..0x08abd828` read by the field camera modes. */

void GameFieldCameraStaticInit(void)
{
  g_gameFieldCameraAxisConsts.axisX.x = 1.0f;
  g_gameFieldCameraAxisConsts.axisX.y = 0.0f;
  g_gameFieldCameraAxisConsts.axisX.z = 0.0f;
  g_gameFieldCameraAxisConsts.axisX.w = 0.0f;
  g_gameFieldCameraAxisConsts.axisY.x = 0.0f;
  g_gameFieldCameraAxisConsts.axisY.y = 1.0f;
  g_gameFieldCameraAxisConsts.axisY.z = 0.0f;
  g_gameFieldCameraAxisConsts.axisY.w = 0.0f;
  g_gameFieldCameraAxisConsts.axisZ.x = 0.0f;
  g_gameFieldCameraAxisConsts.axisZ.y = 0.0f;
  g_gameFieldCameraAxisConsts.axisZ.z = 1.0f;
  g_gameFieldCameraAxisConsts.axisZ.w = 0.0f;
  g_gameFieldCameraAxisConsts.zero.x = 0.0f;
  g_gameFieldCameraAxisConsts.zero.y = 0.0f;
  g_gameFieldCameraAxisConsts.zero.z = 0.0f;
  g_gameFieldCameraAxisConsts.zero.w = 0.0f;
  g_gameFieldCameraAxisConsts.up.x = 0.0f;
  g_gameFieldCameraAxisConsts.up.y = 1.0f;
  g_gameFieldCameraAxisConsts.up.z = 0.0f;
  g_gameFieldCameraAxisConsts.up.w = 0.0f;
  g_gameFieldCameraAxisConsts.down.x = 0.0f;
  g_gameFieldCameraAxisConsts.down.y = -1.0f;
  g_gameFieldCameraAxisConsts.down.z = 0.0f;
  g_gameFieldCameraAxisConsts.down.w = 0.0f;
  g_gameFieldCameraAxisConsts.negX.x = -1.0f;
  g_gameFieldCameraAxisConsts.negX.y = 0.0f;
  g_gameFieldCameraAxisConsts.negX.z = 0.0f;
  g_gameFieldCameraAxisConsts.negX.w = 0.0f;
  g_gameFieldCameraAxisConsts.posX.x = 1.0f;
  g_gameFieldCameraAxisConsts.posX.y = 0.0f;
  g_gameFieldCameraAxisConsts.posX.z = 0.0f;
  g_gameFieldCameraAxisConsts.posX.w = 0.0f;
  g_gameFieldCameraAxisConsts.negZ.x = 0.0f;
  g_gameFieldCameraAxisConsts.negZ.y = 0.0f;
  g_gameFieldCameraAxisConsts.negZ.z = -1.0f;
  g_gameFieldCameraAxisConsts.negZ.w = 0.0f;
  g_gameFieldCameraAxisConsts.posZ.x = 0.0f;
  g_gameFieldCameraAxisConsts.posZ.y = 0.0f;
  g_gameFieldCameraAxisConsts.posZ.z = 1.0f;
  g_gameFieldCameraAxisConsts.posZ.w = 0.0f;
  g_gameFieldCameraLookAtHeight = 13.5999994f;        /* 0x41599999 */
  g_gameFieldCameraTalkLookAtHeight = 14.3999996f;    /* 0x41666666 */
  g_gameFieldCameraTerminalLookAtHeight = 14.3999996f;
  g_gameFieldCameraStepLookAtHeight = 14.3999996f;
  g_gameFieldCameraHighDistance = 19.1999989f;        /* 0x41999999 */
  g_gameFieldCameraHighLookAtHeight = 17.6000004f;    /* 0x418ccccd */
  g_gameFieldCameraDefaultDistance = 32.0f;
  g_gameFieldCameraTerminalDistance = 32.0f;
  g_gameFieldCameraTalkDistance = 40.0f;
  g_gameFieldCameraStepDistance = 16.0f;
  g_gameFieldCameraNearDistance = 8.0f;
}
