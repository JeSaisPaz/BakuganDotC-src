// bdc 0x08846994 BtlTutorialSetControlLock
#include "bdc.h"

/* Sets the input-disabled byte of `unit`'s `BtlInput` controller to 1 when `lock` is true, 0
   otherwise; `unit` NULL means the player's Bakugan (`BtlGetPlayerBakugan`). The task argument
   is unused. */

void BtlTutorialSetControlLock(void *task, bool lock, void *unit)
{
    BtlBakugan *target = (BtlBakugan *)unit;

    (void)task;
    if (target == NULL) {
        target = (BtlBakugan *)BtlGetPlayerBakugan();
    }
    if (lock) {
        target->input->disabled = 1;
    } else {
        target->input->disabled = 0;
    }
}
