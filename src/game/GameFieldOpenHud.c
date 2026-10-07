// bdc 0x088bf140 GameFieldOpenHud
#include "bdc.h"

/* Creates the field HUD (`UiFieldHudCtor`, task 3001) if it does not exist and, outside stage
   0x20, sets the player's HUD flag `+0x3c4`. */

void GameFieldOpenHud(void)
{
  ActorPlayer *player;

  if (CoreTaskExists(0xbb9) == 0) {
    CoreTaskCreate(0xbb9, 100);
    if (g_scriptGlobalVars[1] != 0x20) {
      player = ActorFindPlayer();
      if (player != NULL) {
        player->powersEnabled = 1;
      }
    }
  }
}
