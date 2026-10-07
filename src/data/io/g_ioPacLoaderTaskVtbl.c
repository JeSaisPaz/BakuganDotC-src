// bdc 0x08af591c g_ioPacLoaderTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_ioPacLoaderTaskVtbl = {
    {0}, { .fn = (void *)IoPacLoaderTaskDtor }, { .fn = (void *)IoPacLoaderTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
