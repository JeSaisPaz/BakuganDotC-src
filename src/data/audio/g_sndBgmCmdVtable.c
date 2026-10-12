// bdc 0x08af526c g_sndBgmCmdVtable
#include "bdc.h"

__typeof__(VtblEntry[7]) g_sndBgmCmdVtable = {
    {0}, { .fn = (void *)SndBgmCmdDestroy }, { .fn = (void *)SndBgmCmdUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
