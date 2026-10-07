// bdc 0x08af6f40 g_gameQuestCamDirWatchBackVtbl
#include "bdc.h"

__typeof__(VtblEntry[5]) g_gameQuestCamDirWatchBackVtbl = {
    {0}, { .fn = (void *)GameQuestCamDirWatchBackEnterNop },
    { .fn = (void *)GameQuestCamDirWatchBackExitNop },
    { .fn = (void *)GameQuestCamDirWatchBackNodeChangedNop },
    { .fn = (void *)GameQuestCamDirWatchBackUpdate },
};
