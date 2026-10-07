// bdc 0x088bf1a0 GameFieldCloseHud
#include "bdc.h"

/* Removes the field HUD task 3001 (`CoreTaskRemove`) if it exists and clears the player's HUD
   flag `powersEnabled` (`+0x3c4`). */

void GameFieldCloseHud(void)

{
  ActorPlayer *player;
  CoreTask *task;

  player = (ActorPlayer *)ActorFindPlayer();
  task = (CoreTask *)CoreTaskFind(0xbb9);
  if ((task != (CoreTask *)0x0) && (CoreTaskRemove(task,true), player != (ActorPlayer *)0x0)) {
    player->powersEnabled = 0;
  }
  return;
}
