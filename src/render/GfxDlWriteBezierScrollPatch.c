// bdc 0x089f5ae8 GfxDlWriteBezierScrollPatch
#include "bdc.h"

/* Writes a textured 4x4 Bezier patch into the display list `dl` for a 3D sprite of
   `GfxSpriteLayerDraw3D`, only while the battle default-camera flag `g_btlCameraDefaultMode` is
   set (`BtlCameraEnterDefaultMode` sets it, the demo/cut-in code clears it): UOFFSET (`0x4a`)
   from the scrolling global `g_gfxBezierScrollU`, VOFFSET 0, USCALE (`0x48`) `scaleU`, VSCALE
   12.0, VTYPE `0x118` (4444 colour, 16-bit positions), BASE/VADDR of the fixed control points
   `g_gfxBezierScrollPatchPoints`, `BEZIER` `0x05000404` (4×4 control points), then resets the UV
   scale to 1 and the offsets to 0. Finally stores a PSUB word (`0x36`, `divS | divT << 8`) and
   returns the pointer to that word, so the next command overwrites it. Returns `dl` unchanged when
   the flag is clear. */

/* GE float argument: the top 24 bits of the IEEE single. */
static inline u32 GfxBezierFloatArg(float f)
{
  union { float f; u32 u; } bits;
  bits.f = f;
  return bits.u >> 8;
}

u32 *GfxDlWriteBezierScrollPatch(float scaleU, u32 *dl, u32 divS, u32 divT)
{
  u32 addr;

  if (g_btlCameraDefaultMode != 0) {
    dl[0] = GfxBezierFloatArg(g_gfxBezierScrollU) | 0x4a000000; /* UOFFSET */
    dl[1] = GfxBezierFloatArg(0.0f) | 0x4b000000;               /* VOFFSET */
    dl[2] = GfxBezierFloatArg(scaleU) | 0x48000000;             /* USCALE */
    dl[3] = GfxBezierFloatArg(12.0f) | 0x49000000;              /* VSCALE */
    dl[4] = 0x12000118;                                         /* VTYPE */
    addr = (u32)(uintptr_t)g_gfxBezierScrollPatchPoints;
    dl[5] = ((addr >> 24) & 0xf) << 16 | 0x10000000;            /* BASE */
    dl[6] = (addr & 0xffffff) | 0x01000000;                     /* VADDR */
    dl[7] = 0x05000404;                                         /* BEZIER 4x4 */
    dl[8] = GfxBezierFloatArg(1.0f) | 0x48000000;
    dl[9] = GfxBezierFloatArg(1.0f) | 0x49000000;
    dl[10] = GfxBezierFloatArg(0.0f) | 0x4a000000;
    dl[11] = GfxBezierFloatArg(0.0f) | 0x4b000000;
    dl += 12;
    *dl = divT << 8 | 0x36000000 | divS;                        /* PSUB, not advanced */
  }
  return dl;
}
