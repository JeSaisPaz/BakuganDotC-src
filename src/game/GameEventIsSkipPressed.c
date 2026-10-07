// bdc 0x088eb6d8 GameEventIsSkipPressed
#include "bdc.h"

/* True when the pad state exists and button `0x4000` was pressed this frame (skips messages). */

s32 GameEventIsSkipPressed(void)

{
  if ((g_padState != (PadState *)0x0) && ((g_padState->pressed & 0x4000) != 0)) {
    return 1;
  }
  return 0;
}

