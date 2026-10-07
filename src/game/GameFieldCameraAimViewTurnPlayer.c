// bdc 0x088ccb04 GameFieldCameraAimViewTurnPlayer
#include "bdc.h"

/* Turns the player (`ActorFindPlayer`) while aiming. Without a lock-on target (`lockTarget`)
   the heading `rot[1]` changes by +0.0314 rad per frame with stick X below -0.8 and by -0.0314
   rad with stick X not at or below 0.8 (nothing while `throwing` is set). With a lock-on target
   the heading is set from the player's field camera yaw (π/2 − (yaw + π/2), wrapped once), or
   left alone when the player has no camera. The heading is then wrapped into (−π, π]. `aim` is
   unused. */

void GameFieldCameraAimViewTurnPlayer(void *aim)
{
    ActorPlayer *player;
    float delta;
    float heading;

    (void)aim;
    player = (ActorPlayer *)ActorFindPlayer();
    delta = 0.0f;
    if (g_padState->stickX < -0.8f) {
        delta = 0.03141593f;
    } else if (!(g_padState->stickX <= 0.8f)) {
        delta = -0.03141593f;
    }
    if (player->throwing != 0) {
        delta = 0.0f;
    }
    if (player->lockTarget == NULL) {
        player->base.base.rot[1] = player->base.base.rot[1] + delta;
    } else if (player->base.camera != NULL) {
        heading = 1.5707964f - (player->base.camera->base.yaw + 1.5707964f);
        if (!(heading <= 3.1415927f)) {
            heading = heading - 6.2831855f;
        } else if (heading <= -3.1415927f) {
            heading = heading + 6.2831855f;
        }
        player->base.base.rot[1] = heading;
    }
    heading = player->base.base.rot[1];
    if (!(heading <= 3.1415927f)) {
        player->base.base.rot[1] = heading - 6.2831855f;
        return;
    }
    if (heading <= -3.1415927f) {
        player->base.base.rot[1] = heading + 6.2831855f;
    }
}
