// bdc 0x08a9e928 g_uiCollectionTheaterPhaseFns
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_uiCollectionTheaterPhaseFns = {
    { .pfn = (void *)UiCollectionTheaterPhaseWait },
    { .pfn = (void *)UiCollectionTheaterPhaseLoad },
    { .pfn = (void *)UiCollectionTheaterMainPhase },
    { .pfn = (void *)UiCollectionTheaterPhasePlayScene },
    { .pfn = (void *)UiCollectionTheaterPhaseClose },
};
