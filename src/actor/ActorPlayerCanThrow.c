// bdc 0x088e1450 ActorPlayerCanThrow
#include "bdc.h"

/* True when story event flag 0x3d1 is set (`GameEventIsFlag3d1Set`) and the current stage (script
   global 1, `*(0x08ac58c4 + 4)`) is not 0x20. Gates the throw state in `ActorSetState` and the
   throw icon of the field HUD. */

bool ActorPlayerCanThrow(ActorPlayer *self)
{
    if (g_scriptGlobalVars[1] != 0x20) {
        return GameEventIsFlag3d1Set();
    }
    return false;
}
