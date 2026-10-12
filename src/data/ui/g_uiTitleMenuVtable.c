// bdc 0x08af4db4 g_uiTitleMenuVtable
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiTitleMenuVtable = {
    {0}, { .fn = (void *)UiTitleMenuDtor }, { .fn = (void *)UiTitleMenuUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiTitleMenuDraw },
    { .fn = (void *)UiTitleMenuSetField }, { .fn = (void *)UiTitleMenuGetField },
};
