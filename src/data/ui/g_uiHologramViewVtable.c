// bdc 0x08af4a44 g_uiHologramViewVtable
#include "bdc.h"

__typeof__(VtblEntry[2]) g_uiHologramViewVtable = { {0}, { .fn = (void *)UiHologramViewDtor } };
