// bdc 0x08af45fc g_btlDemoVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_btlDemoVtbl = {
    {0}, { .fn = (void *)BtlDemoDtor }, { .fn = (void *)BtlDemoUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)BtlDemoDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
