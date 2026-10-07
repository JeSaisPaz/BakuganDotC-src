// bdc 0x08941a50 UiNetLobbyStubDoneB
#include "bdc.h"

/* Second stub of UiNetLobby returning 1; same usage as UiNetLobbyStubDoneA.
    */
s32 UiNetLobbyStubDoneB(UiScreen *screen, u8 reset)
{
    (void)screen;
    (void)reset;
    return 1;
}
