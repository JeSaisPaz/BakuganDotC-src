// bdc 0x0885ba00 BtlTargetPointDraw
#include "bdc.h"

/* Empty draw virtual (slot 8) of the `TargetPoint` units and the same three
   other vtables as `BtlTargetPointUpdate`. */
void BtlTargetPointDraw(void *unit)
{
    (void)unit;
}
