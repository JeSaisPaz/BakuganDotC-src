// bdc 0x08af4fac g_uiCollectionCardVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_uiCollectionCardVtbl = { {0}, { .fn = (void *)UiCollectionCardDtor } };
