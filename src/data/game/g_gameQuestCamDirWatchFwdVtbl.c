// bdc 0x08af6f68 g_gameQuestCamDirWatchFwdVtbl
#include "bdc.h"

__typeof__(VtblEntry[5]) g_gameQuestCamDirWatchFwdVtbl = {
    {0}, { .fn = (void *)GameQuestCamDirWatchFwdEnterNop },
    { .fn = (void *)GameQuestCamDirWatchFwdExitNop },
    { .fn = (void *)GameQuestCamDirWatchFwdNodeChangedNop },
    { .fn = (void *)GameQuestCamDirWatchFwdUpdate },
};
