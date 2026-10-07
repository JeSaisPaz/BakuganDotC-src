// bdc 0x08af6250 g_btlAiFollowLayerVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_btlAiFollowLayerVtbl = { {0}, { .fn = (void *)BtlAiFollowLayerReset }, { .fn = (void *)BtlAiLayerIsActive } };
