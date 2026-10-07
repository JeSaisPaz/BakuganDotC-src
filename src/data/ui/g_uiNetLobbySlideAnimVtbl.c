// bdc 0x08af4c34 g_uiNetLobbySlideAnimVtbl
#include "bdc.h"

__typeof__(VtblEntry[6]) g_uiNetLobbySlideAnimVtbl = {
    {0}, { .fn = (void *)UiNetLobbySlideAnimDtor }, { .fn = (void *)UiNetLobbyAnimInit },
    { .fn = (void *)UiNetLobbySlideAnimShow }, { .fn = (void *)UiNetLobbySlideAnimHide },
    { .fn = (void *)UiNetLobbyAnimSetVisible },
};
