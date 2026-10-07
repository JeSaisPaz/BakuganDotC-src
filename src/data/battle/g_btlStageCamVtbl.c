// bdc 0x08af466c g_btlStageCamVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_btlStageCamVtbl = {
    {0}, { .fn = (void *)BtlStageCamDtor }, { .fn = (void *)BtlStageCamUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
