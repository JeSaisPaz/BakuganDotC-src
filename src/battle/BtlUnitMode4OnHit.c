// bdc 0x0885dbe0 BtlUnitMode4OnHit
#include "bdc.h"

/* Hit-reaction virtual of the mode-4 battle unit (vtable `0x08af1e1c` slot `+0xc0`, overriding
   `BtlBakuganOnHit`): reads the attack id (`hitKind`) from the body collider `collider0`;
   attacks 0x1a..0x1f whose id − 0x1a equals the unit's attribute (virtual slot 20) are ignored.
   For a hit by a unit (collider `hitParam164` 1..3, attacker `hitAttacker`) with an attack listed
   by `BtlUnitMode4IsCountedAttackId`, adds 1 to counter 0x14 of the attacker's `stats`
   (`BtlStatsAddCounter`) when it has one. Then runs `BtlBakuganOnHit` and, when `keepHpFull`
   is set and the unit is dead (`combat.dead`), revives it at full HP (`BtlCombatReset`,
   `BtlCombatGetMaxHp` converted to float as unsigned, `BtlCombatSetHp`). */

void BtlUnitMode4OnHit(BtlUnitMode4 *self)
{
    CollisionCollider *body = (CollisionCollider *)self->base.collider0;
    s32 attackId = body->hitKind;
    BtlBakugan *attacker = NULL;
    s32 hitParam;

    if (attackId >= 0x1a && attackId < 0x20) {
        const VtblEntry *entry = &((const VtblEntry *)self->base.base.base.vtable)[20];

        if (((int (*)(void *))entry->fn)((u8 *)self + entry->delta) == attackId - 0x1a) {
            return;
        }
    }
    body = (CollisionCollider *)self->base.collider0;
    hitParam = body->hitParam164;
    if (hitParam < 2) {
        if (hitParam > 0) {
            attacker = (BtlBakugan *)body->hitAttacker;
        }
    } else if (hitParam < 4) {
        attacker = (BtlBakugan *)body->hitAttacker;
    }
    if (attacker != NULL && BtlUnitMode4IsCountedAttackId(self, attackId) != 0 &&
        attacker->stats != NULL) {
        BtlStatsAddCounter(attacker->stats, 0x14, 1);
    }
    BtlBakuganOnHit(&self->base);
    if (self->keepHpFull != 0 && self->base.combat.dead != 0) {
        BtlCombatReset(&self->base.combat);
        BtlCombatSetHp((float)(u32)BtlCombatGetMaxHp(&self->base.combat), &self->base.combat);
    }
}
