// bdc 0x08af4a0c g_uiHologramGalleryVtable
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiHologramGalleryVtable = {
    {0}, { .fn = (void *)UiHologramGalleryDtor }, { .fn = (void *)UiHologramGalleryUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiHologramGalleryDraw },
    { .fn = (void *)UiHologramGallerySetField }, { .fn = (void *)UiHologramGalleryGetField },
};
