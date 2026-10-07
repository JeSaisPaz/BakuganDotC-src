// bdc 0x089ed1a8 GfxPaletteBlend
#include "bdc.h"

/* Blends the colour entries `start .. start+rangeCount` of CLUT rows `rowA` and `rowB` of the palette
   blender's texture (`GfxTextureGetClutData`, rows of `count` RGBA8 entries) by `t` into the
   blender's `output` CLUT, then writes `count` entries of the output back from the data cache.
   Per entry and per byte (VFPU in the original): both channels are unpacked to floats
   (`vuc2i.s` + `vi2f.q …, 31`, about byte/255), lerped a + (b - a) * t, clamped to [0, 1]
   (`vsat0.q`), scaled by 255 (bank constant S701) and repacked (`vf2iz.q …, 23` + `vi2uc.q`). */

void GfxPaletteBlend(float t, GfxPaletteBlender *pb, s32 rowA, s32 rowB)
{
  u32 *clut;
  const u32 *srcA;
  const u32 *srcB;
  u32 *dst;
  s32 i;
  s32 lane;
  u32 wa;
  u32 wb;
  u32 packed;
  float a;
  float b;

  clut = (u32 *)GfxTextureGetClutData((GfxTexture *)pb->texture);
  i = pb->start;
  if (i < i + pb->rangeCount) {
    srcA = clut + pb->count * rowA + i;
    srcB = clut + pb->count * rowB + i;
    dst = (u32 *)pb->output + i;
    do {
      wa = *srcA;
      wb = *srcB;
      packed = 0;
      for (lane = 0; lane < 4; lane++) {
        a = (float)(s32)(((wa >> (lane * 8)) & 0xffu) * 0x01010101u >> 1) / 2147483648.0f;
        b = (float)(s32)(((wb >> (lane * 8)) & 0xffu) * 0x01010101u >> 1) / 2147483648.0f;
        a = a + (b - a) * t;
        packed |= (u32)VfI2uc(VfF2iz(VfSat0(a) * 255.0f, 23)) << (lane * 8);
      }
      *dst = packed;
      i++;
      srcA++;
      srcB++;
      dst++;
    } while (i < pb->start + pb->rangeCount);
  }
  sceKernelDcacheWritebackInvalidateRange(pb->output, pb->count << 2);
  return;
}
