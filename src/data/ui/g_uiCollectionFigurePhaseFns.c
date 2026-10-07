// bdc 0x08a9e9f0 g_uiCollectionFigurePhaseFns
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiCollectionFigurePhaseFns = {
    { .pfn = (void *)UiCollectionFigurePhaseWait }, { .pfn = (void *)UiCollectionFigurePhaseLoad },
    { .pfn = (void *)UiCollectionFigureMainPhase }, { .pfn = (void *)UiCollectionFigurePhaseClose },
};
