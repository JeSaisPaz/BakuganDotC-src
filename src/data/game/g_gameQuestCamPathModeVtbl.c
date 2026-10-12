// bdc 0x08af4414 g_gameQuestCamPathModeVtbl
#include "bdc.h"

__typeof__(VtblEntry[8]) g_gameQuestCamPathModeVtbl = {
    {0}, { .fn = (void *)GameQuestCamPathModeDtor }, { .fn = (void *)GameQuestCamTargetGetLookAt },
    { .fn = (void *)GameQuestCamObjSetLookAtNop }, { .fn = (void *)GameQuestCamModeResetSmooth },
    { .fn = (void *)GameQuestCamPathModeUpdateTarget },
    { .fn = (void *)GameQuestCamPathModeUpdateEye },
    { .fn = (void *)GameQuestCamPathModeGetSegmentPoint },
};
