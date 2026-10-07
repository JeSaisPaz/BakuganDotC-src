// bdc 0x08ab9f10 g_gfxPuffKindTable
#include "bdc.h"

__typeof__(MemberFnPtr[8]) g_gfxPuffKindTable = {
    { .pfn = (void *)GfxPuffKindNone }, { .pfn = (void *)GfxPuffKindDust },
    { .pfn = (void *)GfxPuffKindDarkSmoke }, { .pfn = (void *)GfxPuffKindRisingSmoke },
    { .pfn = (void *)GfxPuffKindLight }, { .pfn = (void *)GfxPuffKindGlow },
    { .pfn = (void *)GfxPuffKindFlash }, { .pfn = (void *)GfxPuffKindShockwave },
};
