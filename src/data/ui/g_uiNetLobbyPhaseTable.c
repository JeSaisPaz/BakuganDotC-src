// bdc 0x08a9cf78 g_uiNetLobbyPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_uiNetLobbyPhaseTable = {
    { .pfn = (void *)UiNetLobbyIdlePhase }, { .pfn = (void *)UiNetLobbyHostPhase },
    { .pfn = (void *)UiNetLobbyJoinPhase }, { .pfn = (void *)UiNetLobbyMessagePhase },
    { .pfn = (void *)UiNetLobbyExitPhase },
};
