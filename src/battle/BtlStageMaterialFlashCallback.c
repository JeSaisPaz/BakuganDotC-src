// bdc 0x0889cbb4 BtlStageMaterialFlashCallback
#include "bdc.h"

/* Material animation callback installed by `BtlStageMaterialSetFlash`: fades a stage material
   out by `*value` (0..1). With `a = 255 - (int)(*value * 255) > 0` it appends alpha test
   `0xdbff0006`, material alpha `0x58` = `a`, material ambient `0x5cffffff`, `0x17000001`,
   `0x1f000001` and the alpha test again; otherwise material alpha 0, `0x17000001`, `0xe7000001`
   and alpha test `0xdbff0000` (never passes), hiding the material. Each word is appended at `*dl`,
   which advances by one. */
void BtlStageMaterialFlashCallback(u32 **dl, const float *value)
{
    s32 alpha = 0xff - (s32)(*value * 255.0f);

    if (alpha > 0) {
        *(*dl)++ = 0xdbff0006;
        *(*dl)++ = (u32)alpha | 0x58000000;
        *(*dl)++ = 0x5cffffff;
        *(*dl)++ = 0x17000001;
        *(*dl)++ = 0x1f000001;
        *(*dl)++ = 0xdbff0006;
        return;
    }
    *(*dl)++ = 0x58000000;
    *(*dl)++ = 0x17000001;
    *(*dl)++ = 0xe7000001;
    *(*dl)++ = 0xdbff0000;
}
