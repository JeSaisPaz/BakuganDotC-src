// bdc 0x089f2bfc GfxPacketDrawRect
#include "bdc.h"

/* Draws a solid-colour rectangle into a render packet: `GfxPacketBeginChunk`, write the rectangle
   with `GfxDlDrawColorRect` (two s16 corner vertices from `rect = {x, y, w, h}` floats, a few GE
   state commands, the colour from the RGBA `vec4` clamped and packed with VFPU `vf2iz`/`vi2uc` into
   the material-colour commands, and a sprite PRIM), then `GfxPacketEndChunk`. */

void GfxPacketDrawRect(RenderPacket *packet, const float *rect, const float *color)

{
  u32 *list;
  
  list = GfxPacketBeginChunk(packet);
  list = GfxDlDrawColorRect(packet,list,rect,(ScePspFVector4 *)color);
  GfxPacketEndChunk(packet,list);
  return;
}

