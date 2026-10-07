// bdc 0x088e721c ActorNpcStaticInit
#include "bdc.h"

/* Static initialiser (C++ constructor table entry `0x08af5c68`) of the NPC actor unit: stores
   the ten float4 axis constants at `0x08abf630..0x08abf6cf` in order axis X, axis Y, axis Z,
   zero, +Y, -Y, -X, +X, -Z, +Z (every w = 0). No matrix is written. */
void ActorNpcStaticInit(void)
{
  g_actorNpcAxisConsts.axisX.x = 1.0f;
  g_actorNpcAxisConsts.axisX.y = 0.0f;
  g_actorNpcAxisConsts.axisX.z = 0.0f;
  g_actorNpcAxisConsts.axisX.w = 0.0f;
  g_actorNpcAxisConsts.axisY.x = 0.0f;
  g_actorNpcAxisConsts.axisY.y = 1.0f;
  g_actorNpcAxisConsts.axisY.z = 0.0f;
  g_actorNpcAxisConsts.axisY.w = 0.0f;
  g_actorNpcAxisConsts.axisZ.x = 0.0f;
  g_actorNpcAxisConsts.axisZ.y = 0.0f;
  g_actorNpcAxisConsts.axisZ.z = 1.0f;
  g_actorNpcAxisConsts.axisZ.w = 0.0f;
  g_actorNpcAxisConsts.zero.x = 0.0f;
  g_actorNpcAxisConsts.zero.y = 0.0f;
  g_actorNpcAxisConsts.zero.z = 0.0f;
  g_actorNpcAxisConsts.zero.w = 0.0f;
  g_actorNpcAxisConsts.up.x = 0.0f;
  g_actorNpcAxisConsts.up.y = 1.0f;
  g_actorNpcAxisConsts.up.z = 0.0f;
  g_actorNpcAxisConsts.up.w = 0.0f;
  g_actorNpcAxisConsts.down.x = 0.0f;
  g_actorNpcAxisConsts.down.z = 0.0f;
  g_actorNpcAxisConsts.down.y = -1.0f;
  g_actorNpcAxisConsts.down.w = 0.0f;
  g_actorNpcAxisConsts.negX.x = -1.0f;
  g_actorNpcAxisConsts.negX.y = 0.0f;
  g_actorNpcAxisConsts.negX.z = 0.0f;
  g_actorNpcAxisConsts.negX.w = 0.0f;
  g_actorNpcAxisConsts.posX.x = 1.0f;
  g_actorNpcAxisConsts.posX.y = 0.0f;
  g_actorNpcAxisConsts.posX.z = 0.0f;
  g_actorNpcAxisConsts.posX.w = 0.0f;
  g_actorNpcAxisConsts.negZ.x = 0.0f;
  g_actorNpcAxisConsts.negZ.y = 0.0f;
  g_actorNpcAxisConsts.negZ.z = -1.0f;
  g_actorNpcAxisConsts.negZ.w = 0.0f;
  g_actorNpcAxisConsts.posZ.x = 0.0f;
  g_actorNpcAxisConsts.posZ.y = 0.0f;
  g_actorNpcAxisConsts.posZ.z = 1.0f;
  g_actorNpcAxisConsts.posZ.w = 0.0f;
}
