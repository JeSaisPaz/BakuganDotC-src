// bdc 0x089f0a44 CollisionDebugPrimsDraw
#include "bdc.h"

/* Draws every debug primitive of the chain `g_collisionDebugPrims` into one chunk of the render
   packet `list` (nothing when the chain is empty). The chunk CALLs the state list
   `g_collisionDebugStateDl` (patch primitive = lines), sets depth test from
   `g_collisionDebugDepthTest`, blend src-alpha/one-minus-src-alpha and the identity world
   matrix. Per primitive: its colour as ambient colour/alpha, its world matrix `mtx` when `hasMtx`
   (the identity is re-sent for the next primitive without one), then by `kind`: 1 a spline patch
   between `from` and `to`, 2/3/4 a closed spline ring around X/Y/Z, 5 the unit box (line strip
   plus 3 lines), 6 a CALL of its `displayList`, otherwise a single line `from`-`to`. Each
   primitive's `frames` is decremented and the primitive deleted (virtual dtor, flags 3) when it
   reaches 0 or below. The chunk ends by restoring patch primitive triangles. */

/* GE BASE command for the upper address bits of `p`. */
#define GE_BASE(p) (0x10000000u | (u32)((((uintptr_t)(p) >> 24) & 0xf) << 16))
/* GE address command `cmd` (VADDR 1, IADDR 2, JUMP 8, CALL 0xa) with the low 24 bits of `p`. */
#define GE_ADDR(cmd, p) (((u32)(cmd) << 24) | (u32)((uintptr_t)(p) & 0xffffff))

void CollisionDebugPrimsDraw(void *list)

{
  CollisionDebugPrim *prim;
  CollisionDebugPrim *next;
  const ScePspFVector4 *rows;
  const VtblEntry *vt;
  float *verts;
  u32 *dl;
  int worldSet;
  int i;
  union {
    float f;
    u32 u;
  } bits;

  prim = g_collisionDebugPrims;
  if (prim == NULL) {
    return;
  }
  dl = GfxPacketBeginChunk(list);
  dl[0] = GE_BASE(g_collisionDebugStateDl);
  dl[1] = GE_ADDR(0x0a, g_collisionDebugStateDl);
  dl[2] = g_collisionDebugDepthTest != 0 ? 0x23000001 : 0x23000000; /* ZTE */
  dl[3] = 0xdf000032; /* ALPHA: src alpha, one minus src alpha, add */
  dl[4] = 0xe0000000; /* SFIX */
  dl[5] = 0xe1000000; /* DFIX */
  dl[6] = 0x3a000000; /* WMS 0 */
  /* WORLD data: 4 rows x 3 floats of the identity, each float shifted down 8 bits */
  rows = &g_gfxIdentityMatrix.x;
  for (i = 0; i < 4; i++) {
    bits.f = rows[i].x;
    dl[7 + i * 3] = (g_collisionDebugWorldCmd & 0xff000000) | (bits.u >> 8);
    bits.f = rows[i].y;
    dl[8 + i * 3] = (g_collisionDebugWorldCmd & 0xff000000) | (bits.u >> 8);
    bits.f = rows[i].z;
    dl[9 + i * 3] = (g_collisionDebugWorldCmd & 0xff000000) | (bits.u >> 8);
  }
  dl += 19;
  worldSet = 0;
  do {
    next = (CollisionDebugPrim *)prim->base.next;
    dl[0] = (prim->colour & 0xffffff) | 0x55000000; /* AMC */
    dl[1] = (prim->colour >> 24) | 0x58000000;      /* AMA */
    dl += 2;
    rows = NULL;
    if (prim->hasMtx != 0) {
      worldSet = 1;
      rows = &prim->mtx.x;
    }
    else if (worldSet != 0) {
      worldSet = 0;
      rows = &g_gfxIdentityMatrix.x;
    }
    if (rows != NULL) {
      dl[0] = 0x3a000000; /* WMS 0 */
      for (i = 0; i < 4; i++) {
        bits.f = rows[i].x;
        dl[1 + i * 3] = (g_collisionDebugWorldCmd & 0xff000000) | (bits.u >> 8);
        bits.f = rows[i].y;
        dl[2 + i * 3] = (g_collisionDebugWorldCmd & 0xff000000) | (bits.u >> 8);
        bits.f = rows[i].z;
        dl[3 + i * 3] = (g_collisionDebugWorldCmd & 0xff000000) | (bits.u >> 8);
      }
      dl += 13;
    }
    switch (prim->kind) {
    case 2:
    case 3:
    case 4:
      dl[0] = 0x12000980; /* VTYPE: float position, 8-bit index */
      dl[1] = GE_BASE(g_collisionDebugRingIdx);
      dl[2] = GE_ADDR(0x02, g_collisionDebugRingIdx);
      if (prim->kind == 2) {
        dl[3] = GE_BASE(g_collisionDebugRingYZ);
        dl[4] = GE_ADDR(0x01, g_collisionDebugRingYZ);
      }
      else if (prim->kind == 3) {
        dl[3] = GE_BASE(g_collisionDebugRingXZ);
        dl[4] = GE_ADDR(0x01, g_collisionDebugRingXZ);
      }
      else {
        dl[3] = GE_BASE(g_collisionDebugRingXY);
        dl[4] = GE_ADDR(0x01, g_collisionDebugRingXY);
      }
      dl[5] = 0x060c0409; /* SPLINE 9x4 control points */
      dl += 6;
      break;
    case 5:
      dl[0] = 0x12000980;
      dl[1] = GE_BASE(g_collisionDebugBoxStripIdx);
      dl[2] = GE_ADDR(0x02, g_collisionDebugBoxStripIdx);
      dl[3] = GE_BASE(g_collisionDebugBoxVerts);
      dl[4] = GE_ADDR(0x01, g_collisionDebugBoxVerts);
      dl[5] = 0x0402000a; /* PRIM line strip, 10 */
      dl[6] = 0x12000980;
      dl[7] = GE_BASE(g_collisionDebugBoxEdgeIdx);
      dl[8] = GE_ADDR(0x02, g_collisionDebugBoxEdgeIdx);
      dl[9] = GE_BASE(g_collisionDebugBoxVerts);
      dl[10] = GE_ADDR(0x01, g_collisionDebugBoxVerts);
      dl[11] = 0x04010006; /* PRIM lines, 6 */
      dl += 12;
      break;
    case 1:
      /* inline vertices from, to: jump over them */
      verts = (float *)(dl + 2);
      dl[0] = GE_BASE(dl + 8);
      dl[1] = GE_ADDR(0x08, dl + 8);
      verts[0] = prim->from.x;
      verts[1] = prim->from.y;
      verts[2] = prim->from.z;
      verts[3] = prim->to.x;
      verts[4] = prim->to.y;
      verts[5] = prim->to.z;
      dl += 8;
      dl[0] = 0x12000980;
      dl[1] = GE_BASE(g_collisionDebugLineIdx);
      dl[2] = GE_ADDR(0x02, g_collisionDebugLineIdx);
      dl += 3;
      if (verts != NULL) {
        dl[0] = GE_BASE(verts);
        dl[1] = GE_ADDR(0x01, verts);
        dl += 2;
      }
      *dl++ = 0x060f0404; /* SPLINE 4x4 control points */
      break;
    case 6:
      dl[0] = GE_BASE(prim->displayList);
      dl[1] = GE_ADDR(0x0a, prim->displayList);
      dl += 2;
      break;
    default:
      verts = (float *)(dl + 2);
      dl[0] = GE_BASE(dl + 8);
      dl[1] = GE_ADDR(0x08, dl + 8);
      verts[0] = prim->from.x;
      verts[1] = prim->from.y;
      verts[2] = prim->from.z;
      verts[3] = prim->to.x;
      verts[4] = prim->to.y;
      verts[5] = prim->to.z;
      dl += 8;
      *dl++ = 0x12000180; /* VTYPE: float position, no index */
      if (verts != NULL) {
        dl[0] = GE_BASE(verts);
        dl[1] = GE_ADDR(0x01, verts);
        dl += 2;
      }
      *dl++ = 0x04010002; /* PRIM lines, 2 */
      break;
    }
    prim->frames = prim->frames - 1; /* bgtz on the decremented value */
    if (prim->frames < 1) {
      vt = (const VtblEntry *)prim->base.vtable + 1;
      ((void (*)(void *, u32))vt->fn)((char *)prim + vt->delta, 3);
    }
    prim = next;
  } while (prim != NULL);
  *dl = 0x37000000; /* PPRIM triangles */
  GfxPacketEndChunk(list, dl + 1);
}
