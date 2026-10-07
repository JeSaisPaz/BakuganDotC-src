// bdc 0x08af4b24 g_uiUnlockResultVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiUnlockResultVtbl = {
    {0}, { .fn = (void *)UiUnlockResultDtor }, { .fn = (void *)UiUnlockResultUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiUnlockResultDraw },
    { .fn = (void *)UiUnlockResultSetField }, { .fn = (void *)UiUnlockResultGetField },
};
