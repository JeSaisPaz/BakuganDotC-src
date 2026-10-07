// bdc 0x0885dd58 BtlBakuganAreOthersAllDead
#include "bdc.h"

/* Returns 1 when every unit in `g_btlBakuganList` other than `self` is dead (combat `dead`
   byte set), else 0. Used by `BtlBakuganUpdateTargeting` to stop retargeting when nothing is
   left to fight. */
int BtlBakuganAreOthersAllDead(BtlBakugan *self)
{
    BtlBakugan *unit;

    for (unit = *(BtlBakugan **)g_btlBakuganList; unit != NULL;
         unit = (BtlBakugan *)unit->base.base.next) {
        if (unit != self && unit->combat.dead == 0) {
            return 0;
        }
    }
    return 1;
}
