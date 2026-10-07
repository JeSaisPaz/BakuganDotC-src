// bdc 0x08a29fbc BtlBakuganGetAttribute
#include "bdc.h"

/* Battle-unit virtual (entry 20, fn at `+0xa4`): returns the unit's attribute, the signed byte
   `attribute` of its combat stat block. */
s32 BtlBakuganGetAttribute(BtlBakugan *unit)
{
    return (s32)unit->combat.stats->attribute;
}
