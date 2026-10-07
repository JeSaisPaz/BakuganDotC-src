// bdc 0x08929f84 UiHologramViewStopVoice
#include "bdc.h"

/* Stops the narration voice: `SndBgmCancelChannel`(1) and `SndBgmQueueStop`(0.1, 1). */

void UiHologramViewStopVoice(void)

{
  SndBgmCancelChannel(1);
  SndBgmQueueStop(0.1f,1);
  return;
}

