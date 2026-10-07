// bdc 0x08941a58 UiNetLobbySetActivePanel
#include "bdc.h"

/* Selects which of the two side panels of `UiNetLobby` is visible (`+0xa0`). */
void UiNetLobbySetActivePanel(UiNetLobby *lobby, s32 panel)
{
    lobby->activePanel = panel;
}
