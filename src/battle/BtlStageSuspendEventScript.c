// bdc 0x0889d988 BtlStageSuspendEventScript
#include "bdc.h"

/* Sets the `paused` flag of the battle event script `g_btlEventScript` when it exists; used
   while talk windows or camera sequences run (`UiTalkShowMessage`). Counterpart of
   `BtlStageResumeEventScript`. */

void BtlStageSuspendEventScript(void)

{
  if (g_btlEventScript != NULL) {
    g_btlEventScript->paused = 1;
  }
}

