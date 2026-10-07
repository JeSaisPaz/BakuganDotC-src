// bdc 0x08af4c64 g_uiNetLobbyVtable
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiNetLobbyVtable = {
    {0}, { .fn = (void *)UiNetLobbyDtor }, { .fn = (void *)UiNetLobbyUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiNetLobbyDraw },
    { .fn = (void *)UiNetLobbySetField }, { .fn = (void *)UiNetLobbyGetField },
};
