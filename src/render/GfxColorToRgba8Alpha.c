// bdc 0x08a324b8 GfxColorToRgba8Alpha
#include "bdc.h"

/* Packs a float RGBA colour into a 32-bit `RGBA8` word (red in the low byte) with its alpha
   multiplied by `alpha`: each component (alpha after the multiply) is clamped to [0, 1], scaled by
   255 (the VFPU bank constant S701) and truncated to a byte. */
u32 GfxColorToRgba8Alpha(const ScePspFVector4 *rgba, float alpha)
{
    float a = rgba->w * alpha;

    return (u32)VfI2uc(VfF2iz(VfSat0(rgba->x) * 255.0f, 23)) |
           (u32)VfI2uc(VfF2iz(VfSat0(rgba->y) * 255.0f, 23)) << 8 |
           (u32)VfI2uc(VfF2iz(VfSat0(rgba->z) * 255.0f, 23)) << 16 |
           (u32)VfI2uc(VfF2iz(VfSat0(a) * 255.0f, 23)) << 24;
}
