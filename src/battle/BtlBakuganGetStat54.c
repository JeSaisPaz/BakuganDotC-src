// bdc 0x08865d34 BtlBakuganGetStat54
#include "bdc.h"

/* Returns the unit's raw dash deceleration from its stat table (not speed-scaled); used with
   `BtlBakuganGetScaledStat50` by `BtlBakuganDashStep` and `BtlUnitMode4MoveTowardPoint`. */
float BtlBakuganGetStat54(BtlBakugan *bakugan)
{
    return bakugan->combat.stats->dashDecel;
}
