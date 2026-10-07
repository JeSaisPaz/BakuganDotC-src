// bdc 0x08a9dc88 g_uiCollectionSpherePhaseFns
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiCollectionSpherePhaseFns = {
    { .pfn = (void *)UiCollectionSpherePhaseWait }, { .pfn = (void *)UiCollectionSpherePhaseLoad },
    { .pfn = (void *)UiCollectionSphereMainPhase }, { .pfn = (void *)UiCollectionSpherePhaseClose },
};
