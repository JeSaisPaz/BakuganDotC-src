// bdc 0x08886cfc BtlCombatHasUpgrade
#include "bdc.h"

/* Returns 1 if the unit has upgrade `upgradeId`, i.e. any of its six equipped slot ids equals it,
   else 0. `BtlCombatLoadLoadout` fills the ids from the Bakugan's owned upgrade slots
   (`UiUpgradeGetUpgradeId`); `BtlCombatInit` zeroes them. */
int BtlCombatHasUpgrade(BtlCombatState *combat, int upgradeId)
{
    int i;

    for (i = 0; i < 6; i++) {
        if (combat->slotIds[i] == upgradeId) {
            return 1;
        }
    }
    return 0;
}
