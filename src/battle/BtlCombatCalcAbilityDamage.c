// bdc 0x088877ac BtlCombatCalcAbilityDamage
#include "bdc.h"

/* Damage of an ability/attack-object hit (hit classes 2..3). Returns 0 for a NULL attacker and
   hands attack ids below 0x23 to `BtlCalcHazardDamage`. Otherwise it copies the
   `BtlAttackParams` of type `attackId - 0x23`, takes the attacker's
   `BtlCombatGetAttackFactor` (× 1.5 for types 0, 0x3a and 0x3b while status 6 is active) and the
   attacker's attribute (the stat table's `attribute` when virtual slot 10 or slot 12 returns
   nonzero, else the result of virtual slot 20 when slot 11 returns nonzero, else 6 = none), and
   returns factor × `BtlGetAttributeMultiplier``(0, attr, special, targetAttr)` × power, clamped
   to at least 0. Power is `power × hitCount` for a normal hit (`special == 0`) and
   `specialPower × 0.7` otherwise. */
float BtlCombatCalcAbilityDamage(BtlBakugan *attacker, s32 hitClass, s32 attackId, s32 special, s32 targetAttr)
{
    BtlAttackParams params;
    const VtblEntry *entry;
    s32 type;
    s32 attackerAttr;
    float damage;

    (void)hitClass;
    if (attacker == NULL) {
        return 0.0f;
    }
    if (attackId < 0x23) {
        return BtlCalcHazardDamage(attackId, special, targetAttr);
    }
    type = attackId - 0x23;
    BtlAttackParamsCopy(&params, type);
    damage = BtlCombatGetAttackFactor(&attacker->combat);
    if (type == 0 || type == 0x3a || type == 0x3b) {
        if (attacker->combat.status[6].active != 0) {
            damage = damage * 1.5f;
        }
    }

    attackerAttr = 6;
    entry = &((const VtblEntry *)attacker->base.base.vtable)[10];
    if (((int (*)(void *))entry->fn)((u8 *)attacker + entry->delta) != 0) {
        attackerAttr = attacker->combat.stats->attribute;
    } else {
        entry = &((const VtblEntry *)attacker->base.base.vtable)[12];
        if (((int (*)(void *))entry->fn)((u8 *)attacker + entry->delta) != 0) {
            attackerAttr = attacker->combat.stats->attribute;
        } else {
            entry = &((const VtblEntry *)attacker->base.base.vtable)[11];
            if (((int (*)(void *))entry->fn)((u8 *)attacker + entry->delta) != 0) {
                entry = &((const VtblEntry *)attacker->base.base.vtable)[20];
                attackerAttr = ((s32 (*)(void *))entry->fn)((u8 *)attacker + entry->delta);
            }
        }
    }

    damage = damage * BtlGetAttributeMultiplier(0, attackerAttr, special, targetAttr);
    if (special == 0) {
        damage = damage * (float)params.power * (float)params.hitCount;
    } else {
        damage = damage * (float)params.specialPower * 0.699999988f;
    }
    if (damage < 0.0f) {
        damage = 0.0f;
    }
    return damage;
}
