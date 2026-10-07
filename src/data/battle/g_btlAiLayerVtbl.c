// bdc 0x08af6220 g_btlAiLayerVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_btlAiLayerVtbl = { {0}, { .fn = (void *)BtlAiLayerReset }, { .fn = (void *)BtlAiLayerIsActive } };
