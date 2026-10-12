// bdc 0x08af4dec g_uiTitleVtable
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiTitleVtable = {
    {0}, { .fn = (void *)UiTitleDtor }, { .fn = (void *)UiTitleUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiTitleDraw },
    { .fn = (void *)UiTitleSetField }, { .fn = (void *)UiTitleGetField },
};
