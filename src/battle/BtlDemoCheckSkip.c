// bdc 0x088ffd6c BtlDemoCheckSkip
#include "bdc.h"

/* Returns the demo skip request: 1 when `endRequest` is set, else 0; raised to 2 when profile flag
   0 is clear (`SaveGetProfileFlag0`), no dialog is busy (`BtlIsDialogBusy`) and START (0x0008)
   or CROSS (0x4000) was newly pressed on the demo's pad (`pad->pressed`), or when profile flag 0 is
   set, a profile exists (`SaveHasProfile`) and it has flag 0x80 (`SaveProfileHasFlags`). */

int BtlDemoCheckSkip(BtlDemo *demo)
{
    int request;

    request = demo->endRequest != 0;
    if (SaveGetProfileFlag0() == 0 && !BtlIsDialogBusy()) {
        /* byte loads of pressed: bit 3 (START) of the low byte, bit 6 (CROSS) of the high byte */
        if ((demo->pad->pressed & 0x0008) != 0 || (demo->pad->pressed & 0x4000) != 0) {
            request = 2;
        }
    }
    if (SaveGetProfileFlag0() != 0 && SaveHasProfile() &&
        SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
        request = 2;
    }
    return request;
}
