// bdc 0x0884d67c BtlMainCheckPlayerDefeated
#include "bdc.h"

/* Lose check of `BtlMainCheckLose`: when the player's Bakugan (`BtlGetPlayerBakugan`) exists,
   its vtable entry 14 returns 0 and its combat state is `dead`, starts the finish cinematic on it
   (`BtlStartFinishCinematic` mode 0) and returns 1. Otherwise, in battle rule mode 1 (script
   global variable 8) with a nonzero `standingTargetLimit`, when the count of standing target
   objects (`ActorStageObjCountStandingTargets`) is below that limit it sets profile word 0x1c to
   1 and starts the finish cinematic (mode 1) on `g_gameLastActivatedObject`, returning 1.
   Returns 0 otherwise. */

int BtlMainCheckPlayerDefeated(BtlMain *self)
{
    BtlBakugan *player;
    int lost = 0;

    player = BtlGetPlayerBakugan();
    if (player != NULL) {
        const VtblEntry *isTargetPoint = &((const VtblEntry *)player->base.base.vtable)[14];

        if (((int (*)(void *))isTargetPoint->fn)((u8 *)player + isTargetPoint->delta) == 0 &&
            player->combat.dead) {
            lost = 1;
            BtlStartFinishCinematic(self, player, 1, 0);
        }
    }
    if (!lost && g_scriptGlobalVars[8] == 1 && self->standingTargetLimit != 0 &&
        ActorStageObjCountStandingTargets() < self->standingTargetLimit) {
        SaveProfileSetWord(SaveGetProfile(), 0x1c, 1);
        lost = 1;
        BtlStartFinishCinematic(self, g_gameLastActivatedObject, 1, 1);
    }
    return lost;
}
