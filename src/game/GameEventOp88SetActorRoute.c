// bdc 0x088f11ac GameEventOp88SetActorRoute
#include "bdc.h"

/* Handler of event opcode 0x88 (`GameEvent470ExecCommand`): puts character `flag` on route `arg`
   (`GameFieldCharSetSetActorRoute`). Flag 100 selects the talk partner. */

void GameEventOp88SetActorRoute(GameEvent470 *self, u8 flag, s16 arg)

{
  u32 idx;

  idx = flag;
  if (idx == 100) {
    idx = self->talkPartner;
  }
  GameFieldCharSetSetActorRoute(g_gameFieldCharSet,self->actorMap[idx],(u8)arg,'\0');
  return;
}
