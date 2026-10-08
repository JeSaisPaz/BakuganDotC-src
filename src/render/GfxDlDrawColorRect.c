// bdc 0x089f2c58 GfxDlDrawColorRect
#include "bdc.h"

/* Writes a solid-colour rectangle into the display list and returns the advanced pointer: a
   BASE+JUMP over two inline through-mode s16 vertices (`{x, y, 0}` and `{x + w, y + h, 0}` from
   `rect = {x, y, w, h}` floats, truncated), texture off, alpha blend (`0xdf`/`0xe0`/`0xe1`), the
   material/ambient colour packed from `colour` (`0x55`/`0x58`), vertex type `0x12800100`, BASE+VADDR
   to the vertices and a 2-vertex sprite PRIM (`0x04060002`). The first argument is unused. */

u32 *GfxDlDrawColorRect(void *unused, u32 *list, const float *rect, const ScePspFVector4 *colour)
{
  s16 *verts;
  u32 *body;
  u32 addr;
  u32 packed;

  (void)unused;
  verts = (s16 *)(list + 2);
  body = list + 5;
  addr = PspAddr(body);
  list[0] = ((addr >> 24) & 0xf) << 16 | 0x10000000;
  list[1] = (addr & 0xffffff) | 0x08000000;
  verts[0] = (s16)(s32)rect[0];
  verts[1] = (s16)(s32)rect[1];
  verts[2] = 0;
  verts[3] = (s16)(s32)(rect[0] + rect[2]);
  verts[4] = (s16)(s32)(rect[1] + rect[3]);
  verts[5] = 0;
  body[0] = 0x1e000000;
  body[1] = 0xdf000032;
  body[2] = 0xe0000000;
  body[3] = 0xe1000000;
  body += 4;
  packed = (u32)VfI2uc(VfF2iz(VfSat0(colour->x) * 255.0f, 23)) |
           (u32)VfI2uc(VfF2iz(VfSat0(colour->y) * 255.0f, 23)) << 8 |
           (u32)VfI2uc(VfF2iz(VfSat0(colour->z) * 255.0f, 23)) << 16 |
           (u32)VfI2uc(VfF2iz(VfSat0(colour->w) * 255.0f, 23)) << 24;
  body[0] = (packed & 0xffffff) | 0x55000000;
  body[1] = (packed >> 24) | 0x58000000;
  body[2] = 0x12800100;
  body += 3;
  if (verts != NULL) {
    addr = PspAddr(verts);
    body[0] = ((addr >> 24) & 0xf) << 16 | 0x10000000;
    body[1] = (addr & 0xffffff) | 0x01000000;
    body += 2;
  }
  body[0] = 0x04060002;
  return body + 1;
}
