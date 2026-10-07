// bdc 0x08af1834 g_btlTutorialTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_btlTutorialTaskVtbl = {
    {0}, { .fn = (void *)BtlTutorialTaskDtor }, { .fn = (void *)BtlTutorialTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
