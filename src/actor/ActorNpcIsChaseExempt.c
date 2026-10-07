// bdc 0x088e7660 ActorNpcIsChaseExempt
#include "bdc.h"

/* Per-stage exceptions for the chase: returns 1 when the stage (script global 1) and the NPC's
   route index (`record+0x3b`) match one of the hard-coded pairs (stage 4: 0x96; 8: 0xa4; 0xc:
   0xa0/0xa4/0xa5; 0xe: 0xa0/0xa2). Used by `ActorNpcStateCatchPlayer`. */

s32 ActorNpcIsChaseExempt(ActorNpc *self)
{
    u8 idx = ((ActorNpcPlacement *)self->base.placement)->routeIndex;

    switch (g_scriptGlobalVars[1]) {
    case 4:
        if (idx == 0x96) {
            return 1;
        }
        break;
    case 8:
        if (idx == 0xa4) {
            return 1;
        }
        break;
    case 0xc:
        if (idx == 0xa0 || idx == 0xa4 || idx == 0xa5) {
            return 1;
        }
        break;
    case 0xe:
        if (idx == 0xa0 || idx == 0xa2) {
            return 1;
        }
    }
    return 0;
}
