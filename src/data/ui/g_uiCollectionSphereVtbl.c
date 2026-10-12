// bdc 0x08af4f74 g_uiCollectionSphereVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiCollectionSphereVtbl = {
    {0}, { .fn = (void *)UiCollectionSphereDtor }, { .fn = (void *)UiCollectionSphereUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiCollectionSphereDraw },
    { .fn = (void *)UiCollectionSphereSetField }, { .fn = (void *)UiCollectionSphereGetField },
};
