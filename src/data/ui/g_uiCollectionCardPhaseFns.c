// bdc 0x08a9e838 g_uiCollectionCardPhaseFns
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiCollectionCardPhaseFns = {
    { .pfn = (void *)UiCollectionCardPhaseWait }, { .pfn = (void *)UiCollectionCardPhaseLoad },
    { .pfn = (void *)UiCollectionCardMainPhase }, { .pfn = (void *)UiCollectionCardPhaseClose },
};
