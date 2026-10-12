// bdc 0x08af44cc g_gameQuestCamLookSpringVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gameQuestCamLookSpringVtbl = {
    {0}, { .fn = (void *)GameQuestCamLookSpringDtor },
    { .fn = (void *)GameQuestCamLookAtBaseGetLookAt },
    { .fn = (void *)GameQuestCamLookSpringSetLookAt }, { .fn = (void *)GameQuestCamObjResetNop },
    { .fn = (void *)GameQuestCamLookSpringUpdateTarget },
    { .fn = (void *)GameQuestCamLookSpringUpdate },
};
