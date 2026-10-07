// bdc 0x088ced98 UiFieldHudIsSuspended
#include "bdc.h"

/* Returns whether the field HUD (task 3001, `UiFieldHudCtor`; sprite array `+0x1c`, player
   `+0x74`) should skip its phase update this frame: when the field task is missing or
   `GameFieldIsPlayerFree` says so, when task 371 (`UiBakuganSelectCtor`) exists, or when the
   HUD is disabled (`g_uiFieldHudEnabled` == 0); in the first cases it also calls
   `ActorPlayerStopPenaltySound(player)`. */

bool UiFieldHudIsSuspended(UiFieldHud *self)

{
  bool suspended;
  CoreTask *task;
  s32 selectExists;
  ActorPlayer *player;

  task = CoreTaskFind(500);
  selectExists = CoreTaskExists(0x173);
  suspended = GameFieldIsPlayerFree(task) != 0 || (selectExists != 0 || task == (CoreTask *)0x0);
  player = self->player;
  if (((player != (ActorPlayer *)0x0) && ((player->base).state == 3)) &&
     ((player->base).state == 8)) {
    suspended = true;
  }
  if (suspended) {
    ActorPlayerStopPenaltySound(player);
  }
  if (g_uiFieldHudEnabled == '\0') {
    suspended = true;
  }
  return suspended;
}
