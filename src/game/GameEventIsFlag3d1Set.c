// bdc 0x088f555c GameEventIsFlag3d1Set
#include "bdc.h"

/* Returns `GameEventFlagTest``(0x3d1)`; used by `ActorPlayerCanThrow`. */

bool GameEventIsFlag3d1Set(void)

{

  
  return GameEventFlagTest(0x3d1);
}

