// bdc 0x08af4504 g_gameQuestCamRailSpringVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gameQuestCamRailSpringVtbl = {
    {0}, { .fn = (void *)GameQuestCamRailSpringDtor },
    { .fn = (void *)GameQuestCamLookAtBaseGetLookAt },
    { .fn = (void *)GameQuestCamObjSetLookAtNop }, { .fn = (void *)GameQuestCamObjResetNop },
    { .fn = (void *)GameQuestCamRailSpringUpdateAxes },
    { .fn = (void *)GameQuestCamRailSpringUpdate },
};
