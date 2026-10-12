// bdc 0x08af4494 g_gameQuestCamEyeSpringVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gameQuestCamEyeSpringVtbl = {
    {0}, { .fn = (void *)GameQuestCamEyeSpringDtor },
    { .fn = (void *)GameQuestCamLookAtBaseGetLookAt },
    { .fn = (void *)GameQuestCamObjSetLookAtNop }, { .fn = (void *)GameQuestCamObjResetNop },
    { .fn = (void *)GameQuestCamEyeSpringUpdateAxes },
    { .fn = (void *)GameQuestCamEyeSpringUpdate },
};
