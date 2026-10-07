// bdc 0x08af5054 g_unlockCodeVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_unlockCodeVtbl = {
    {0}, { .fn = (void *)UiUnlockCodeDtor }, { .fn = (void *)UiUnlockCodeUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiUnlockCodeDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
