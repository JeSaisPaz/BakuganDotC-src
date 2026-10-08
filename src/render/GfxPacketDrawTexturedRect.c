// bdc 0x089f2e54 GfxPacketDrawTexturedRect
#include "bdc.h"

/* Draws a textured screen rectangle into `packet`: state (`GfxDlSetBlendState` with `colour`,
   texture on, blend 1), the texture (`GfxTextureWriteCall`, slot 0), and a through-mode sprite
   (two inline `GfxGeTexVertex16` behind a JUMP) with screen corners `rect + pos` (`rect` =
   x0, y0, x1, y1; `pos` = x, y) and texel coordinates `uv` (u0, v0, u1, v1), all truncated to
   integers. */

void GfxPacketDrawTexturedRect(void *packet, const float *pos, const float *rect, const ScePspFVector4 *uv, void *texture, const ScePspFVector4 *colour)
{
  u32 *list;
  u32 *end;
  GfxGeTexVertex16 *verts;
  GfxGeTexVertex16 *second;
  float corners[4];
  s32 xy[4];
  s32 st[4];

  list = GfxPacketBeginChunk(packet);
  list = GfxDlSetBlendState(list, colour, 1, 1);
  list = GfxTextureWriteCall(texture, list, 0);
  corners[0] = rect[0] + pos[0];
  corners[1] = rect[1] + pos[1];
  corners[2] = rect[2] + pos[0];
  corners[3] = rect[3] + pos[1];
  xy[0] = VfF2iz(corners[0], 0);
  xy[1] = VfF2iz(corners[1], 0);
  xy[2] = VfF2iz(corners[2], 0);
  xy[3] = VfF2iz(corners[3], 0);
  st[0] = VfF2iz(uv->x, 0);
  st[1] = VfF2iz(uv->y, 0);
  st[2] = VfF2iz(uv->z, 0);
  st[3] = VfF2iz(uv->w, 0);
  verts = (GfxGeTexVertex16 *)(list + 2);
  second = verts + 1;
  end = (u32 *)(second + 1);
  /* BASE + JUMP over the inline vertex data */
  list[0] = ((PspAddr(end) >> 0x18) & 0xf) << 0x10 | 0x10000000;
  list[1] = (PspAddr(end) & 0xffffff) | 0x8000000;
  verts[0].u = st[0];
  verts[0].v = st[1];
  second->u = st[2];
  second->v = st[3];
  verts[0].x = xy[0];
  verts[0].y = xy[1];
  second->x = xy[2];
  second->y = xy[3];
  second->z = 0;
  verts[0].z = 0;
  list = end;
  *list++ = 0x12800102; /* VTYPE: 16-bit texel, 16-bit position, through mode */
  if (verts != NULL) {
    list[0] = ((PspAddr(verts) >> 0x18) & 0xf) << 0x10 | 0x10000000;
    list[1] = (PspAddr(verts) & 0xffffff) | 0x1000000; /* VADDR */
    list += 2;
  }
  *list = 0x4060002; /* PRIM sprites, 2 vertices */
  GfxPacketEndChunk(packet, list + 1);
}
