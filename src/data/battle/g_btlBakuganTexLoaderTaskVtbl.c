// bdc 0x08af212c g_btlBakuganTexLoaderTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_btlBakuganTexLoaderTaskVtbl = {
    {0}, { .fn = (void *)BtlBakuganTexLoaderTaskDtor },
    { .fn = (void *)BtlBakuganTexLoaderTaskUpdate }, { .fn = (void *)CoreTaskBaseSlot3Nop },
    { .fn = (void *)CoreTaskBaseDraw }, { .fn = (void *)CoreTaskSetField },
    { .fn = (void *)CoreTaskGetField },
};
