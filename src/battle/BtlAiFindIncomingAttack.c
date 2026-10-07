// bdc 0x08892e50 BtlAiFindIncomingAttack
#include "bdc.h"

/* Returns the first attack in `g_btlAttackList` that is guardable (`BtlAttackParamsIsFlag08Clear`
   on its `type`), has not ended (`endFrame` 0), has an `owner`, and whose owner's `targetId` is the
   id of the `BtlAi`'s own unit; NULL when none matches. */
CoreObject *BtlAiFindIncomingAttack(BtlAi *self)
{
    CoreObject *obj;

    for (obj = g_btlAttackList; obj != NULL; obj = obj->next) {
        BtlAttack *attack = (BtlAttack *)obj;

        if (BtlAttackParamsIsFlag08Clear(attack->type) == 0) {
            continue;
        }
        if (attack->endFrame != 0) {
            continue;
        }
        if (attack->owner == NULL) {
            continue;
        }
        if (attack->owner->targetId == self->owner->base.base.id) {
            break;
        }
    }
    return obj;
}
