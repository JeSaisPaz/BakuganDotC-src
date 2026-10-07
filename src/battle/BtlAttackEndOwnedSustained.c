// bdc 0x08876ef8 BtlAttackEndOwnedSustained
#include "bdc.h"

/* Walks `g_btlAttackList` and ends every sustained attack owned by `owner` whose type is 0xc,
   0x2b, 0x44, 0x48, 0x5b, 0x61, 0x69, 0x6f, 0x76 or 0x7b: `BtlAttackEnd`, then stops the
   effects attached to its `pos` on `g_btlAttackEffectMgr` (`GfxEffectStopAttached`, any id).
   The next node is read before the attack is ended. */

void BtlAttackEndOwnedSustained(void *owner)
{
    BtlAttack *attack;
    BtlAttack *next;

    if (g_btlAttackList == NULL) {
        return;
    }
    for (attack = (BtlAttack *)g_btlAttackList; attack != NULL; attack = next) {
        next = (BtlAttack *)attack->base.next;
        switch (attack->type) {
        case 0x0c:
        case 0x2b:
        case 0x44:
        case 0x48:
        case 0x5b:
        case 0x61:
        case 0x69:
        case 0x6f:
        case 0x76:
        case 0x7b:
            if (attack->owner == owner) {
                BtlAttackEnd(attack);
                GfxEffectStopAttached(g_btlAttackEffectMgr, -1, attack->pos);
            }
            break;
        default:
            break;
        }
    }
}
