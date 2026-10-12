// bdc 0x08af43d4 g_gameQuestCamFixedModeVtbl
#include "bdc.h"

__typeof__(VtblEntry[8]) g_gameQuestCamFixedModeVtbl = {
    {0}, { .fn = (void *)GameQuestCamFixedModeDtor }, { .fn = (void *)GameQuestCamTargetGetLookAt },
    { .fn = (void *)GameQuestCamObjSetLookAtNop }, { .fn = (void *)GameQuestCamObjResetNop },
    { .fn = (void *)GameQuestCamFixedModeCopyTarget },
    { .fn = (void *)GameQuestCamFixedModeUpdate }, { .fn = (void *)GameQuestCamTargetGetPoint },
};
