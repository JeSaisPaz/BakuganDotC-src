// bdc 0x08a9bed8 g_uiHologramGalleryPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_uiHologramGalleryPhaseTable = {
    { .pfn = (void *)UiHologramGalleryLoadPhase }, { .pfn = (void *)UiHologramGalleryFadeInPhase },
    { .pfn = (void *)UiHologramGalleryMainPhase },
    { .pfn = (void *)UiHologramGalleryOpenViewPhase },
    { .pfn = (void *)UiHologramGalleryExitPhase },
};
