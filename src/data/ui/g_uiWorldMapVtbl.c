// bdc 0x08af50c4 g_uiWorldMapVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiWorldMapVtbl = {
    {0}, { .fn = (void *)UiWorldMapDtor }, { .fn = (void *)UiWorldMapUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiWorldMapDraw },
    { .fn = (void *)UiWorldMapSetField }, { .fn = (void *)UiWorldMapGetField },
};
