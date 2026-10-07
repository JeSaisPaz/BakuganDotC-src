// bdc 0x08af4634 g_btlAppearDemoVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_btlAppearDemoVtbl = {
    {0}, { .fn = (void *)BtlAppearDemoDtor }, { .fn = (void *)BtlAppearDemoUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)BtlAppearDemoDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
