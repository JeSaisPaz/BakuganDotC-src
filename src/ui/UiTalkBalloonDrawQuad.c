// bdc 0x088ca978 UiTalkBalloonDrawQuad
#include "bdc.h"

/* Writes a GE chunk for one highlight bar: opens a chunk on `packet`, jumps over an inline copy of
   the 8-vertex 3-slice template `g_uiTalkBalloonBarVerts` (0xa0 bytes) whose last four vertices
   are moved right by `extraWidth`, then emits alpha blending, the material colour/alpha packed
   from `color` (each lane clamped to [0, 1], scaled by 255 and truncated), a world matrix of the identity rotation
   (`g_gfxIdentityMatrix`) with translation `pos`, VTYPE float UV + float position, BASE/VADDR
   of the copied vertices and an 8-vertex triangle strip, and closes the chunk. */

void UiTalkBalloonDrawQuad(float extraWidth, void *packet, float *pos, float *color)
{
  u32 *chunk;
  float *verts;
  u32 *dl;
  const u32 *ident;
  const u32 *tr;
  u32 addr;
  u32 packed;
  int row;
  int col;

  chunk = GfxPacketBeginChunk(packet);
  verts = (float *)(chunk + 2);
  dl = chunk + 2 + 40;
  addr = (u32)(uintptr_t)dl;
  chunk[0] = ((addr >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
  chunk[1] = (addr & 0xffffff) | 0x08000000;          /* JUMP over the vertices */
  memcpy(verts, g_uiTalkBalloonBarVerts, 0xa0);
  verts[4 * 5 + 2] += extraWidth; /* x of vertices 4..7 */
  verts[5 * 5 + 2] += extraWidth;
  verts[6 * 5 + 2] += extraWidth;
  verts[7 * 5 + 2] += extraWidth;

  dl[0] = 0xdf000032; /* BLEND: src alpha, 1 - src alpha, add */
  dl[1] = 0xe0000000; /* FIXA */
  dl[2] = 0xe1000000; /* FIXB */
  dl += 3;

  /* Colour pack: lane = clamp(color * 255) to a byte (S701 is the bank's 255.0). */
  packed = 0;
  for (col = 0; col < 4; col++) {
    packed |= (u32)VfI2uc(VfF2iz(VfSat0(color[col]) * 255.0f, 23)) << (col * 8);
  }
  dl[0] = (packed & 0xffffff) | 0x55000000; /* material colour */
  dl[1] = (packed >> 24) | 0x58000000;      /* material alpha */
  dl += 2;

  /* Inlined world-matrix upload: rows 0..2 of the identity matrix, row 3 = `pos`, each float
     shifted into the 24-bit GE format under command 0x3b (WORLDMATRIXDATA). */
  ident = (const u32 *)&g_gfxIdentityMatrix;
  tr = (const u32 *)pos;
  dl[0] = 0x3a000000; /* WORLDMATRIXNUMBER 0 */
  for (row = 0; row < 3; row++) {
    for (col = 0; col < 3; col++) {
      dl[1 + row * 3 + col] = 0x3b000000 | (ident[row * 4 + col] >> 8);
    }
  }
  dl[10] = 0x3b000000 | (tr[0] >> 8);
  dl[11] = 0x3b000000 | (tr[1] >> 8);
  dl[12] = 0x3b000000 | (tr[2] >> 8);
  dl += 13;

  dl[0] = 0x12000183; /* VTYPE: float UV, float position */
  dl += 1;
  if (verts != NULL) {
    addr = (u32)(uintptr_t)verts;
    dl[0] = ((addr >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
    dl[1] = (addr & 0xffffff) | 0x01000000;          /* VADDR */
    dl += 2;
  }
  dl[0] = 0x04040008; /* PRIM triangle strip x8 */
  GfxPacketEndChunk(packet, dl + 1);
}
