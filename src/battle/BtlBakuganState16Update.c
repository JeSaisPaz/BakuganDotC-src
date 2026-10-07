// bdc 0x08875b70 BtlBakuganState16Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 16 (`+0x140`), vtable slot `+0x150` called through
   `BtlBakuganRunState`. Revive/respawn state (entered from the KO state 6), stepped by
   `subTimer`: 0 clears `respawnCountdown` and the link (`BtlBakuganClearLink`) and waits 30
   frames with the velocity zeroed and gravity off; 2 puts
   the unit on `spawnPoint` (heading from its `w`), restores the default camera for the local
   player (`BtlCameraSetDefaultFollow`, unless task 0x14a exists), stops effects and attacks and
   arms `respawnCountdown = 150`; 10 counts it down (30 extra per frame while command bit 0x8000
   is held); 100 plays sound 0x200012 and spawns respawn effect `0xe + attribute` (virtual slot
   20) on `g_btlUnitEffectMgr`, then waits 8 frames; 102 resets combat (`BtlCombatReset`),
   gravity and the collider flags, returns to state 0, re-targets (`BtlBakuganRetarget`) and
   arms the respawn protection. Other `subTimer` values return without doing anything. */
void BtlBakuganState16Update(BtlBakugan *self)
{
    GfxEffectMgr *mgr;
    const VtblEntry *entry;
    s32 attribute;

    switch (self->subTimer) {
    case 0:
        self->respawnCountdown = 0;
        BtlBakuganClearLink(self);
        self->subWait = 30;
        self->subTimer = self->subTimer + 1;
        /* fallthrough */
    case 1:
        self->base.velocity[0] = 0.0f;
        self->base.velocity[1] = 0.0f;
        self->base.velocity[2] = 0.0f;
        self->base.velocity[3] = 0.0f;
        self->gravity = 0.0f;
        self->gravityHold = 999;
        if (self->subWait-- > 0) {
            return;
        }
        self->subTimer = self->subTimer + 1;
        /* fallthrough */
    case 2:
        BtlBakuganSetHeading(self, self->spawnPoint[3]);
        self->base.pos[0] = self->spawnPoint[0];
        self->base.pos[1] = self->spawnPoint[1];
        self->base.pos[2] = self->spawnPoint[2];
        self->base.pos[3] = self->spawnPoint[3];
        if (BtlBakuganIsLocalPlayer(self) && CoreTaskExists(0x14a) == 0 &&
            g_gfxActiveCamera != NULL) {
            BtlCameraSetDefaultFollow((BtlCamera *)g_gfxActiveCamera, 1);
        }
        BtlBakuganStopEffectsAndAttacks(self);
        self->subTimer = 10;
        self->respawnCountdown = 150;
        /* fallthrough */
    case 10:
        if ((self->commands & 0x8000) != 0) {
            self->respawnCountdown = self->respawnCountdown - 30;
        }
        self->gravityHold = 999;
        self->respawnCountdown = self->respawnCountdown - 1;
        if (self->respawnCountdown > 0) {
            return;
        }
        self->respawnCountdown = 0;
        self->subTimer = self->subTimer + 1;
        /* fallthrough */
    case 11:
        self->subTimer = 100;
        /* fallthrough */
    case 100:
        if (SndHasListener()) {
            SndEmitterCreateAtPos(SndGetListener(), 0x200012, self->base.pos, 0, 1);
        }
        entry = &((const VtblEntry *)self->base.base.vtable)[20];
        mgr = g_btlUnitEffectMgr;
        attribute = ((int (*)(void *))entry->fn)((u8 *)self + entry->delta);
        GfxEffectSpawn(mgr, attribute + 0xe, self->base.pos);
        self->subTimer = self->subTimer + 1;
        self->subWait = 8;
        /* fallthrough */
    case 101:
        self->gravityHold = 999;
        if (self->subWait-- > 0) {
            return;
        }
        self->subTimer = self->subTimer + 1;
        /* fallthrough */
    case 102:
        BtlCombatReset(&self->combat);
        self->gravityHold = 0;
        self->base.ambient[3] = 1.0f;
        if (self->collider0 != NULL) {
            self->collider0->flags &= ~7u;
        }
        if (self->collider1 != NULL) {
            self->collider1->flags &= ~7u;
        }
        self->collider0->hitTimer = 0;
        self->collider0->flags &= ~1u;
        self->collider0->flags &= ~4u;
        self->collider1->flags &= ~0x40u;
        self->collider1->flags &= ~4u;
        BtlShadowUpdate((void **)self->shadow);
        self->gravity = self->combat.stats->gravity;
        BtlBakuganSetState(self, 0, 0);
        BtlBakuganRetarget(self, 1);
        self->subTimer = self->subTimer + 1;
        self->respawnProtect = 1;
        self->respawnStage = 0;
        self->respawnTimer = 0;
        self->collider0->flags |= 1;
        self->collider0->hitTimer = 0x44;
        break;
    default:
        break;
    }
}
