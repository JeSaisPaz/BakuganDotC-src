// bdc 0x08a9db18 g_uiCollectionMenuPhaseFns
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_uiCollectionMenuPhaseFns = {
    { .pfn = (void *)UiCollectionMenuPhaseStart }, { .pfn = (void *)UiCollectionMenuPhaseLoad },
    { .pfn = (void *)UiCollectionMenuMainPhase }, { .pfn = (void *)UiCollectionMenuPhaseSphere },
    { .pfn = (void *)UiCollectionMenuPhaseCard }, { .pfn = (void *)UiCollectionMenuPhaseFigure },
    { .pfn = (void *)UiCollectionMenuPhaseTheater },
    { .pfn = (void *)UiCollectionMenuPhaseUnlockCode },
    { .pfn = (void *)UiCollectionMenuPhaseClose },
};
