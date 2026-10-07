// bdc 0x088898a4 BtlCombatTakeHit
#include "bdc.h"

/* Calls virtual `slot` (a no-argument predicate/getter) of `unit` through its GCC 2.x vtable entry. */
static int UnitVirtual(void *unit, int slot)
{
    const VtblEntry *entry = &((const VtblEntry *)((BtlBakugan *)unit)->base.base.vtable)[slot];

    return ((int (*)(void *))entry->fn)((u8 *)unit + entry->delta);
}

/* True when the unit owning `combat` answers virtual slot 10 or slot 12 (`+0x50`, `+0x60`). */
static int OwnerIsSlot10Or12(BtlCombatState *combat)
{
    return UnitVirtual(combat->owner, 10) != 0 || UnitVirtual(combat->owner, 12) != 0;
}

/* Applies one hit from `attacker` (a unit, may be NULL) to the `BtlCombatState` `combat` of the
   defending unit. When the defender exists, its attribute becomes the target attribute `targetAttr`: from its stat
   table (`stats->attribute`, kept when the table is NULL; `special` = 0) if it answers virtual
   slot 10 or 12, else from virtual slot 20 (`special` = 1) if it answers slot 11; otherwise both
   stay as passed. The damage comes from `BtlCalcDamage`. Hit class 1 (normal) and 2..3
   (special) multiply it by `BtlCombatGetDamageTakenFactor`, then apply
   `BtlCombatApplyDefenseItemBonus` when the defender answers slot 10 and
   `BtlCombatApplyAttackItemBonus` when the attacker is non-NULL and answers slot 10. Class 1
   sets `flag` to `attackId == 0x86` (forced on for ids 0xb3/0xb4) and, when the defender answers
   slot 10 or 12, records the hit on the attacker (`BtlBakuganDecayComboDamageScale`, skipped
   for ids 0x48, 0x53, 0x86, 0x87, 0x93, 0xb3..0xb7) and multiplies the damage by the attacker's
   `comboDamageScale`. Classes 2..3 set `flag` for ids 0x1a..0x1f and 0xa1..0xa5. Any other class
   sets `flag` and uses the raw damage. Finally `BtlCombatApplyDamage``(damage, combat, -1,
   flag)`. */
void BtlCombatTakeHit(BtlCombatState *combat, void *attacker, s32 hitClass, s32 attackId, u8 flag, s32 special, s32 targetAttr)
{
    BtlBakugan *atk = attacker;
    float damage;

    if (combat->owner != NULL) {
        if (OwnerIsSlot10Or12(combat)) {
            if (combat->stats != NULL) {
                targetAttr = combat->stats->attribute;
            }
            special = 0;
        } else if (UnitVirtual(combat->owner, 11) != 0) {
            targetAttr = UnitVirtual(combat->owner, 20);
            special = 1;
        }
    }

    if (hitClass == 1) {
        flag = attackId == 0x86;
        damage = BtlCalcDamage(attacker, hitClass, attackId, special, targetAttr);
        damage = damage * BtlCombatGetDamageTakenFactor(combat);
        if (UnitVirtual(combat->owner, 10) != 0) {
            damage = BtlCombatApplyDefenseItemBonus(damage, combat, attackId, 0);
        }
        if (atk != NULL && UnitVirtual(atk, 10) != 0) {
            damage = BtlCombatApplyAttackItemBonus(damage, &atk->combat, attackId, 0);
        }
        if (attackId == 0xb3 || attackId == 0xb4) {
            flag = 1;
        }
        switch (attackId) {
        case 0x48: case 0x53: case 0x86: case 0x87: case 0x93:
        case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
            break;
        default:
            if (OwnerIsSlot10Or12(combat)) {
                BtlBakuganDecayComboDamageScale(atk, attackId);
            }
            break;
        }
        if (OwnerIsSlot10Or12(combat)) {
            damage = damage * atk->comboDamageScale;
        }
    } else if (hitClass == 2 || hitClass == 3) {
        damage = BtlCalcDamage(attacker, hitClass, attackId, special, targetAttr);
        damage = damage * BtlCombatGetDamageTakenFactor(combat);
        if (UnitVirtual(combat->owner, 10) != 0) {
            damage = BtlCombatApplyDefenseItemBonus(damage, combat, attackId, 1);
        }
        if (atk != NULL && UnitVirtual(atk, 10) != 0) {
            damage = BtlCombatApplyAttackItemBonus(damage, &atk->combat, attackId, 1);
        }
        if (attackId >= 0x1a && attackId < 0x20) {
            flag = 1;
        }
        if (attackId >= 0xa1 && attackId < 0xa6) {
            flag = 1;
        }
    } else {
        flag = 1;
        damage = BtlCalcDamage(attacker, hitClass, attackId, special, targetAttr);
    }
    BtlCombatApplyDamage(damage, combat, -1, flag);
}
