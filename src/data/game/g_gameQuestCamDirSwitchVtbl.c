// bdc 0x08af6f90 g_gameQuestCamDirSwitchVtbl
#include "bdc.h"

__typeof__(VtblEntry[5]) g_gameQuestCamDirSwitchVtbl = {
    {0}, { .fn = (void *)GameQuestCamDirSwitchEnter },
    { .fn = (void *)GameQuestCamDirSwitchExitNop },
    { .fn = (void *)GameQuestCamDirSwitchNodeChangedNop },
    { .fn = (void *)GameQuestCamDirSwitchUpdate },
};
