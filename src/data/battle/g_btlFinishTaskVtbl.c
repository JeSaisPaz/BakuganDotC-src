// bdc 0x08af1884 g_btlFinishTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_btlFinishTaskVtbl = {
    {0}, { .fn = (void *)BtlFinishTaskDtor }, { .fn = (void *)BtlFinishTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)BtlFinishTaskDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
