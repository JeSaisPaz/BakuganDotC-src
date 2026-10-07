// bdc 0x08af2194 g_uiHpGaugeVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_uiHpGaugeVtbl = { {0}, { .fn = (void *)UiHpGaugeDtor } };
