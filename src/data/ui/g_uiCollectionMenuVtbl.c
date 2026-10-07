// bdc 0x08af4f3c g_uiCollectionMenuVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiCollectionMenuVtbl = {
    {0}, { .fn = (void *)UiCollectionMenuDtor }, { .fn = (void *)UiCollectionMenuUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiCollectionMenuDraw },
    { .fn = (void *)UiCollectionMenuSetField }, { .fn = (void *)UiCollectionMenuGetField },
};
