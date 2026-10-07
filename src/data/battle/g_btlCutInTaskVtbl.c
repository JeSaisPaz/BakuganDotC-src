// bdc 0x08af192c g_btlCutInTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_btlCutInTaskVtbl = {
    {0}, { .fn = (void *)BtlCutInTaskDtor }, { .fn = (void *)BtlCutInTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)BtlCutInTaskDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
