// bdc 0x089f2dac GfxPacketDrawScreenFlash
#include "bdc.h"

/* Draws an additive full-screen rectangle (static vertex pair `g_screenFlashVerts`) in `colour` with the
   additive blend preset (`GfxDlSetBlendState` blend 1) into `packet`. */

void GfxPacketDrawScreenFlash(RenderPacket *packet, const ScePspFVector4 *colour)

{
  u32 *list;
  
  list = GfxPacketBeginChunk(packet);
  list = GfxDlSetBlendState(list,colour,0,1);
  *list = 0x12800100;
  list[1] = 0x10080000;
  list[2] = 0x1aa3aca;
  list[3] = 0x4060002;
  GfxPacketEndChunk(packet,list + 4);
  return;
}

