// bdc 0x088feae0 BtlAppearDemoMaterialCallback
#include "bdc.h"

/* `GfxModelForEachMaterial` callback of the Bakugan-appearance demo (`BtlAppearDemoStatePlay`,
   on the appearing ball model): clears bits 5-7 of material flag byte 3 and sets bits 0-1 of flag
   byte 4 to 2. */
void BtlAppearDemoMaterialCallback(u8 *matState)
{
    matState[3] &= 0x1f;
    matState[4] = (matState[4] & 0xfc) | 2;
}
