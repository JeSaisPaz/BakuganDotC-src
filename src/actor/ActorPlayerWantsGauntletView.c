// bdc 0x088e1ef8 ActorPlayerWantsGauntletView
#include "bdc.h"

/* True when the command flags `+0x168` hold bit `0x20000000` and the stage (script global 1) is not
   0x20; the idle state then enters state 11 (`ActorPlayerStateHoldRFocusPoint`). */

bool ActorPlayerWantsGauntletView(ActorPlayer *self)

{
  if ((((self->base).motion & 0x20000000U) != 0) && (g_scriptGlobalVars[1] != 0x20)) {
    return true;
  }
  return false;
}
