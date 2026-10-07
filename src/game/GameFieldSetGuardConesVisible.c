// bdc 0x088e9f94 GameFieldSetGuardConesVisible
#include "bdc.h"

/* Sets the view-cone visibility byte (`*(npc+0x418) + 0x25`) of every guard of the field's NPC list
   (`GameFieldFindTask()+0x628`, placement `+0x37`) that is not in state 6. */

void GameFieldSetGuardConesVisible(void *blind, u8 visible)

{
  ActorNpc *npc;

  for (npc = ((GameFieldTask *)GameFieldFindTask())->npcs; npc != NULL;
       npc = (ActorNpc *)npc->base.base.base.next) {
    if (((ActorNpcPlacement *)npc->base.placement)->placed == 0) {
      continue;
    }
    if (npc->aiState != 6) {
      ((ActorNpcViewCone *)npc->viewCone)->visible = visible;
    }
  }
}
