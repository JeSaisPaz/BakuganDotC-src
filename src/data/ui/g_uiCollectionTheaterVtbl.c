// bdc 0x08af4fe4 g_uiCollectionTheaterVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiCollectionTheaterVtbl = {
    {0}, { .fn = (void *)UiCollectionTheaterDtor }, { .fn = (void *)UiCollectionTheaterUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiCollectionTheaterDraw },
    { .fn = (void *)UiCollectionTheaterSetField }, { .fn = (void *)UiCollectionTheaterGetField },
};
