// bdc 0x08af18f4 g_btlMainVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_btlMainVtbl = {
    {0}, { .fn = (void *)BtlMainTaskDtor }, { .fn = (void *)BtlMainUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)BtlMainDraw },
    { .fn = (void *)BtlMainSetField }, { .fn = (void *)BtlMainGetField },
};
