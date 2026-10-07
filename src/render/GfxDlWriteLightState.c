// bdc 0x089f1460 GfxDlWriteLightState
#include "bdc.h"

/* Writes the scene lighting state into a GE display list: a BASE+CALL to g_gfxLightBaseStateList,
   then a BASE+CALL to g_gfxLightLitStateList (`shadowed` non-zero, light select 0x5f000001) or
   g_gfxLightUnlitStateList (0x5f000002), the select word also stored in g_gfxLightSelectCmd; the
   first time it also patches the specular power command (0x5b, 5.0f) into word 9 of the lit list.
   Then the light 0/1 vectors (g_gfxLightDir0/1 xyz as float24 commands 0x63..0x68), the light 1
   colour g_gfxLightColor1 packed into command 0x93 (each channel clamped to [0, 1], scaled by 255
   and truncated to a byte), and 0xc6000101. Returns the advanced list
   pointer, or the pointer GfxCameraDlWrite returns after appending all of `camera`'s state when
   `camera` is non-NULL. */

static u32 FloatBits(float f)
{
  union { float f; u32 u; } v;
  v.f = f;
  return v.u;
}

u32 *GfxDlWriteLightState(u32 *list, GfxCamera *camera, s32 shadowed)
{
  u32 base = (u32)(uintptr_t)g_gfxLightBaseStateList;
  u32 sub;
  u32 packed;

  list[0] = (((base >> 24) & 0xf) << 16) | 0x10000000;
  list[1] = (base & 0xffffff) | 0x0a000000;
  if (g_gfxLightStateInit == 0) {
    g_gfxLightStateInit = 1;
    g_gfxLightLitStateList[9] = (FloatBits(5.0f) >> 8) | 0x5b000000;
  }
  if (shadowed == 0) {
    sub = (u32)(uintptr_t)g_gfxLightUnlitStateList;
    list[2] = (((sub >> 24) & 0xf) << 16) | 0x10000000;
    list[3] = (sub & 0xffffff) | 0x0a000000;
    g_gfxLightSelectCmd = 0x5f000002;
  } else {
    sub = (u32)(uintptr_t)g_gfxLightLitStateList;
    list[2] = (((sub >> 24) & 0xf) << 16) | 0x10000000;
    list[3] = (sub & 0xffffff) | 0x0a000000;
    g_gfxLightSelectCmd = 0x5f000001;
  }
  list[4] = (FloatBits(g_gfxLightDir0[0]) >> 8) | 0x63000000;
  list[5] = (FloatBits(g_gfxLightDir0[1]) >> 8) | 0x64000000;
  list[6] = (FloatBits(g_gfxLightDir0[2]) >> 8) | 0x65000000;
  list[7] = (FloatBits(g_gfxLightDir1[0]) >> 8) | 0x66000000;
  list[8] = (FloatBits(g_gfxLightDir1[1]) >> 8) | 0x67000000;
  list[9] = (FloatBits(g_gfxLightDir1[2]) >> 8) | 0x68000000;
  packed = (u32)VfI2uc(VfF2iz(VfSat0(g_gfxLightColor1[0]) * 255.0f, 23))
         | ((u32)VfI2uc(VfF2iz(VfSat0(g_gfxLightColor1[1]) * 255.0f, 23)) << 8)
         | ((u32)VfI2uc(VfF2iz(VfSat0(g_gfxLightColor1[2]) * 255.0f, 23)) << 16)
         | ((u32)VfI2uc(VfF2iz(VfSat0(g_gfxLightColor1[3]) * 255.0f, 23)) << 24);
  list[10] = (packed & 0xffffff) | 0x93000000;
  list[11] = 0xc6000101;
  if (camera != NULL) {
    return GfxCameraDlWrite(camera, list + 12, 0xffffffff);
  }
  return list + 12;
}
