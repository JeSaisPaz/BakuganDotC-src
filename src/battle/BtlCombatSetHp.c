// bdc 0x08887aa4 BtlCombatSetHp
#include "bdc.h"

/* Sets a unit's current hit points (`hp`), clamped to `[0, maxHp]`: unless `hp` is below
   `BtlCombatGetMaxHp` (converted as unsigned, so NaN also takes this path) the maximum is stored;
   otherwise a value `<= 0` stores 0. The `dead` flag is then cleared unless the stored HP is
   `<= 0`. Does nothing while the stat table (`stats`) is NULL. */
void BtlCombatSetHp(float hp, BtlCombatState *combat)
{
    float stored;

    if (combat->stats == NULL) {
        return;
    }
    combat->hp = hp;
    if (!(hp < (float)(u32)BtlCombatGetMaxHp(combat))) {
        stored = (float)(u32)BtlCombatGetMaxHp(combat);
        combat->hp = stored;
    } else {
        stored = combat->hp;
        if (stored <= 0.0f) {
            combat->hp = 0.0f;
            stored = 0.0f;
        }
    }
    if (!(stored <= 0.0f)) {
        combat->dead = 0;
    }
}
