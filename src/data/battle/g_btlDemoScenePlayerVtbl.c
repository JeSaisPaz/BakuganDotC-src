// bdc 0x08af46a4 g_btlDemoScenePlayerVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_btlDemoScenePlayerVtbl = {
    {0}, { .fn = (void *)BtlDemoScenePlayerDtor }, { .fn = (void *)BtlDemoScenePlayerUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
