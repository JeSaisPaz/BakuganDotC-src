// bdc 0x08af4454 g_gameQuestCamPointVtbl
#include "bdc.h"

__typeof__(VtblEntry[8]) g_gameQuestCamPointVtbl = {
    {0}, { .fn = (void *)GameQuestCamPointModeDtor }, { .fn = (void *)GameQuestCamTargetGetLookAt },
    { .fn = (void *)GameQuestCamObjSetLookAtNop }, { .fn = (void *)GameQuestCamModeResetSmooth },
    { .fn = (void *)GameQuestCamPointModeUpdateTarget },
    { .fn = (void *)GameQuestCamPointModeUpdateEye },
    { .fn = (void *)GameQuestCamPointModeGetPoint },
};
