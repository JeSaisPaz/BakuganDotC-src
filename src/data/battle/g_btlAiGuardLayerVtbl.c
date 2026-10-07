// bdc 0x08af6238 g_btlAiGuardLayerVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_btlAiGuardLayerVtbl = { {0}, { .fn = (void *)BtlAiGuardLayerReset }, { .fn = (void *)BtlAiLayerIsActive } };
