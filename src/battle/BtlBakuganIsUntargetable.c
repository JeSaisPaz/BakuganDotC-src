// bdc 0x08a29fa8 BtlBakuganIsUntargetable
#include "bdc.h"

/* Battle-unit virtual (entry 17): returns 1 when status 9 is active or the script untargetable
   flag is set, else 0; such units are skipped as targets and opponents. */
int BtlBakuganIsUntargetable(BtlBakugan *unit)
{
    return unit->combat.status[9].active != 0 || unit->untargetable != 0;
}
