// bdc 0x08913e48 UiUpgradeFormatMotionName
#include "bdc.h"

/* Formats the Bakugan-specific motion name: `sprintf(out, table[kind], base)` with the 21-entry
   format table `0x08ac1098` (`"00_dor_%s"`, ...). */

void UiUpgradeFormatMotionName(UiUpgrade *self, char *out, const char *base, u32 kind)
{
    const char *formats[21];

    memcpy(formats, g_uiUpgradeMotionFormats, sizeof(formats));
    sprintf(out, formats[kind & 0xff], base);
}
