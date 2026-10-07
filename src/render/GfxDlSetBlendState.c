// bdc 0x089f1ffc GfxDlSetBlendState
#include "bdc.h"

/* Writes the common flat-draw state: texture enable `tex & 0xff` (`0x1e`), lighting/cull/fog off
   (`0x22`/`0x23`/`0x24`), shade model `0x21000001`, blend preset `blend` (0 alpha, 1 additive,
   2 subtractive, 3 subtractive with reverse equation; `0xdf`/`0xe0`/`0xe1`; any other value writes
   no blend commands) and the material/ambient colour from the RGBA `colour` (`0x55`/`0x58`).
   Returns the advanced list pointer. */

u32 *GfxDlSetBlendState(u32 *list, const ScePspFVector4 *colour, u32 tex, s32 blend)
{
  u32 packed;

  list[0] = (tex & 0xff) | 0x1e000000;
  list[1] = 0x22000000;
  list[2] = 0x23000000;
  list[3] = 0x24000000;
  list[4] = 0x21000001;
  list += 5;
  switch (blend) {
  case 0:
    list[0] = 0xdf0000aa;
    list[1] = 0xe0ffffff;
    list[2] = 0xe1000000;
    list += 3;
    break;
  case 1:
    list[0] = 0xdf000032;
    list[1] = 0xe0000000;
    list[2] = 0xe1000000;
    list += 3;
    break;
  case 2:
    list[0] = 0xdf0000a2;
    list[1] = 0xe0000000;
    list[2] = 0xe1ffffff;
    list += 3;
    break;
  case 3:
    list[0] = 0xdf0002a2;
    list[1] = 0xe0000000;
    list[2] = 0xe1ffffff;
    list += 3;
    break;
  }
  packed = (u32)VfI2uc(VfF2iz(VfSat0(colour->x) * 255.0f, 23)) |
           (u32)VfI2uc(VfF2iz(VfSat0(colour->y) * 255.0f, 23)) << 8 |
           (u32)VfI2uc(VfF2iz(VfSat0(colour->z) * 255.0f, 23)) << 16 |
           (u32)VfI2uc(VfF2iz(VfSat0(colour->w) * 255.0f, 23)) << 24;
  list[0] = (packed & 0xffffff) | 0x55000000;
  list[1] = (packed >> 24) | 0x58000000;
  return list + 2;
}
