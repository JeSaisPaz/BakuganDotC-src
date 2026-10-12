// bdc 0x08af4fac g_uiCollectionCardVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiCollectionCardVtbl = {
    {0}, { .fn = (void *)UiCollectionCardDtor }, { .fn = (void *)UiCollectionCardUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiCollectionCardDraw },
    { .fn = (void *)UiCollectionCardSetField }, { .fn = (void *)UiCollectionCardGetField },
};
