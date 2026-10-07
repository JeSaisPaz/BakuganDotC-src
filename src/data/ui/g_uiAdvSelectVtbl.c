// bdc 0x08af49d4 g_uiAdvSelectVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_uiAdvSelectVtbl = { {0}, { .fn = (void *)UiAdvSelectDtor }, { .fn = (void *)UiAdvSelectUpdate } };
