// bdc 0x088c23d0 GameFieldForwardPlayerFlag
#include "bdc.h"

/* Thin wrapper: calls `ActorSetGuardsViewConeVisible(flag)` (player/actor side); the task argument is unused. */

void GameFieldForwardPlayerFlag(CoreTask *task, u8 flag)

{
  ActorSetGuardsViewConeVisible(flag);
  return;
}

