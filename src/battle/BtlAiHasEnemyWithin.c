// bdc 0x0888efe8 BtlAiHasEnemyWithin
#include "bdc.h"

/* Returns 1 when some Bakugan in the battle list other than the AI's owner (object id differs) is
   targetable (BtlAiIsExcludedTarget false), not dead, without status 9 active, not
   respawn-protected, and closer than `range` to the owner (BtlAiDistanceToUnit); otherwise 0.
   Always 0 in score mode 1 (BtlAiIsScoreMode1) or without a Bakugan list. */

s32 BtlAiHasEnemyWithin(float range, BtlAi *self)
{
    BtlBakugan **list;
    BtlBakugan *unit;

    if (BtlAiIsScoreMode1() != 0) {
        return 0;
    }
    list = (BtlBakugan **)BtlGetBakuganList();
    if (list == NULL) {
        return 0;
    }
    for (unit = *list; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
        if (unit->base.base.id == self->owner->base.base.id) {
            continue;
        }
        if (BtlAiIsExcludedTarget(self, unit)) {
            continue;
        }
        if (unit->combat.dead != 0 || unit->combat.status[9].active != 0 ||
            unit->respawnProtect != 0) {
            continue;
        }
        if (BtlAiDistanceToUnit(self, unit) < range) {
            return 1;
        }
    }
    return 0;
}
