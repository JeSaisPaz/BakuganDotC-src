// bdc 0x08af1504 g_uiLanguageSelectVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiLanguageSelectVtbl = {
    {0}, { .fn = (void *)UiLanguageSelectDtor }, { .fn = (void *)UiLanguageSelectUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiLanguageSelectDraw },
    { .fn = (void *)UiLanguageSelectSetField }, { .fn = (void *)UiLanguageSelectGetField },
};
