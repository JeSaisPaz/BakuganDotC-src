// bdc 0x08870f78 BtlBakuganUpdateRespawnProtection
#include "bdc.h"

/* Respawn protection of a Bakugan, run every frame from `BtlBakuganUpdate`: only in battle rule
   mode 2 (script global 8) with battle type (profile word 7, low byte) 1 or 2, and while
   `respawnProtect` is set. It masks the command bits (`commands &= 0xfff81247`); stage 0 counts
   8 frames, spawns the shield effect 0x37 attached to `anchorMatrix[3]` on `g_worldEffectMgr`
   and advances; stage 1 counts 60 frames, stops the effects attached there, clears
   `respawnProtect` and advances. */
void BtlBakuganUpdateRespawnProtection(BtlBakugan *self)
{
    bool active = false;
    u32 battleType;

    if (g_scriptGlobalVars[8] == 2) {
        battleType = SaveProfileGetWord(SaveGetProfile(), 7) & 0xff;
        if (battleType == 1 || battleType == 2) {
            active = true;
        }
    }
    if (!active || self->respawnProtect == 0) {
        return;
    }
    self->commands = self->commands & 0xfff81247;
    if (self->respawnStage == 0) {
        self->respawnTimer++;
        if (self->respawnTimer >= 8) {
            self->respawnTimer = 0;
            GfxEffectSpawnAttached(g_worldEffectMgr, 0x37, self->anchorMatrix[3]);
            self->respawnStage++;
        }
    } else if (self->respawnStage < 2) {
        self->respawnTimer++;
        if (self->respawnTimer >= 60) {
            self->respawnTimer = 0;
            GfxEffectStopAttached(g_worldEffectMgr, -1, self->anchorMatrix + 3);
            self->respawnProtect = 0;
            self->respawnStage++;
        }
    }
}
