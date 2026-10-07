// bdc 0x08af5134 g_uiPauseSettingsVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiPauseSettingsVtbl = {
    {0}, { .fn = (void *)UiPauseSettingsDtor }, { .fn = (void *)UiPauseSettingsUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiPauseSettingsDraw },
    { .fn = (void *)UiPauseSettingsSetField }, { .fn = (void *)UiPauseSettingsGetField },
};
