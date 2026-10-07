// bdc 0x08892c08 BtlAiUnitStateIs7Or9
#include "bdc.h"

/* Returns 1 when `unit`'s Bakugan state is 7 (melee attack, `BtlBakuganState07Update`) or 9,
   else 0. Condition helper of `BtlAiRunReactionRules`. */
s32 BtlAiUnitStateIs7Or9(BtlAi *self, BtlBakugan *unit)
{
    (void)self;
    if (unit->state != 7 && unit->state != 9) {
        return 0;
    }
    return 1;
}
