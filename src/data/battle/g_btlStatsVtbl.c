// bdc 0x08af1964 g_btlStatsVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_btlStatsVtbl = { {0}, { .fn = (void *)BtlStatsDtor } };
