// bdc 0x08af4c04 g_uiNetLobbyAnimBaseVtable
#include "bdc.h"

__typeof__(VtblEntry[6]) g_uiNetLobbyAnimBaseVtable = {
    {0}, { .fn = (void *)UiNetLobbyAnimBaseDtor }, { .fn = (void *)UiNetLobbyAnimInit },
    { .fn = (void *)UiNetLobbyAnimBaseShow }, { .fn = (void *)UiNetLobbyAnimBaseHide },
    { .fn = (void *)UiNetLobbyAnimSetVisible },
};
