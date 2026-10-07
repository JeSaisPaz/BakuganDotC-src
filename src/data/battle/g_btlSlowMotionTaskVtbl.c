// bdc 0x08af18bc g_btlSlowMotionTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_btlSlowMotionTaskVtbl = {
    {0}, { .fn = (void *)BtlSlowMotionTaskDtor }, { .fn = (void *)BtlSlowMotionTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)BtlSlowMotionTaskDrawNop },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
