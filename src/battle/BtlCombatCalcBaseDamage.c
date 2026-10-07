// bdc 0x08887498 BtlCombatCalcBaseDamage
#include "bdc.h"

/* Base damage of attack `attackId` by unit `attacker`: the attack's power
   (the basic-hit power BtlBakuganGetBasicHitPower(attacker, attackId, special == 0)
   when the unit has a basic-hit row for it, else 20; overridden by fixed values for
   ids 0xb9 = 0, 0x93 = 40, 0x87 = 66, 0x86 = 3, 0x53 = 25, 0x48 = 6, and by basic-hit
   row 0x16/0x17/0x18 for ids 0xb3/0xb4/0xb5..0xb7) times (the attacker's attack
   factor BtlCombatGetAttackFactor(&attacker->combat) times the attribute multiplier
   BtlGetAttributeMultiplier(0, attackerAttr, special, targetAttr)).
   The attacker attribute is the stat table's attribute byte when virtual slot 10 or
   12 answers non-zero, else virtual slot 20's result when slot 11 answers non-zero,
   else 6 (none); it is 6 too for a NULL attacker. */

typedef s32 (*BtlUnitQueryFn)(void *self);

static s32 CallSlot(BtlBakugan *unit, s32 slot)
{
    const VtblEntry *entry = &((const VtblEntry *)unit->base.base.vtable)[slot];
    return ((BtlUnitQueryFn)entry->fn)((u8 *)unit + entry->delta);
}

float BtlCombatCalcBaseDamage(void *attacker, s32 unused, s32 attackId, s32 special, s32 targetAttr)
{
    BtlBakugan *unit = (BtlBakugan *)attacker;
    float power = 20.0f;
    float attackFactor;
    s32 attr;

    (void)unused;
    if (BtlBakuganGetBasicHitIndex(attacker, attackId) != -1) {
        if (special == 0) {
            power = (float)BtlBakuganGetBasicHitPower(attacker, attackId, 1);
        } else {
            power = (float)BtlBakuganGetBasicHitPower(attacker, attackId, 0);
        }
    }

    if (attackId == 0xb9) {
        power = 0.0f;
    } else if (attackId == 0xb4) {
        power = (float)BtlBakuganGetBasicHitRowPower(attacker, 0x17);
    } else if (attackId == 0xb3) {
        power = (float)BtlBakuganGetBasicHitRowPower(attacker, 0x16);
    } else if (attackId == 0x93) {
        power = 40.0f;
    } else if (attackId == 0x87) {
        power = 66.0f;
    } else if (attackId == 0x86) {
        power = 3.0f;
    } else if (attackId == 0x53) {
        power = 25.0f;
    } else if (attackId == 0x48) {
        power = 6.0f;
    }
    if (attackId > 0xb4 && attackId < 0xb8) {
        power = (float)BtlBakuganGetBasicHitRowPower(attacker, 0x18);
    }

    attackFactor = BtlCombatGetAttackFactor(&unit->combat);

    attr = 6;
    if (unit != NULL) {
        if (CallSlot(unit, 10) != 0 || CallSlot(unit, 12) != 0) {
            attr = unit->combat.stats->attribute;
        } else if (CallSlot(unit, 11) != 0) {
            attr = CallSlot(unit, 20);
        }
    }

    return power * (attackFactor * BtlGetAttributeMultiplier(0, attr, special, targetAttr));
}
