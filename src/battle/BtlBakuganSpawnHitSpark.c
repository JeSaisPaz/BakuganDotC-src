// bdc 0x088649a4 BtlBakuganSpawnHitSpark
#include "bdc.h"

/* Spawns the impact effect of a hit at the body collider's hit point on `g_worldEffectMgr`,
   unless the unit is in the no-effect mode (`stateFlags & 0x200000`), the reaction is 4 or 0x14,
   or the collider's hit type is 0. When status 8 is active (guarding) and the hit type is not 3,
   it spawns the guard spark 0x16 and an attached guard effect 0x17 at the unit's anchor
   translation, bound to the unit (`ownerBakugan`/`ownerId`). Otherwise the effect id is 9 without
   an attacker, else the attacker's virtual slot 20 result plus 10 (hitId < 3) or plus 0x10. */

void BtlBakuganSpawnHitSpark(BtlBakugan *self, int reaction, void *attacker, int hitId)
{
    CollisionCollider *collider = (CollisionCollider *)self->collider0;
    s32 hitType = collider->hitType;
    const VtblEntry *getKind;
    GfxEffect *guard;
    int effectId;

    if ((self->stateFlags & 0x200000) != 0 || reaction == 4 || reaction == 0x14 || hitType == 0) {
        return;
    }
    if (self->combat.status[8].active != 0 && hitType != 3) {
        GfxEffectSpawn(g_worldEffectMgr, 0x16, &collider->hitPos.x);
        guard = GfxEffectSpawnAttached(g_worldEffectMgr, 0x17, self->anchorMatrix[3]);
        guard->ownerBakugan = self;
        if (self != NULL) {
            guard->ownerId = self->base.base.id;
        }
        return;
    }
    effectId = 9;
    if (attacker != NULL) {
        getKind = &((const VtblEntry *)((CoreObject *)attacker)->vtable)[20];
        if (hitId < 3) {
            effectId = ((int (*)(void *))getKind->fn)((u8 *)attacker + getKind->delta) + 10;
        } else {
            effectId = ((int (*)(void *))getKind->fn)((u8 *)attacker + getKind->delta) + 0x10;
        }
        collider = (CollisionCollider *)self->collider0;
    }
    GfxEffectSpawn(g_worldEffectMgr, effectId, &collider->hitPos.x);
}
