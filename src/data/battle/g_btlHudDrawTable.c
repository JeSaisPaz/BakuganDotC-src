// bdc 0x08a64bec g_btlHudDrawTable
#include "bdc.h"

__typeof__(MemberFnPtr[10]) g_btlHudDrawTable = {
    { .pfn = (void *)BtlHudDrawLayers }, { .pfn = (void *)BtlHudDrawLayers },
    { .pfn = (void *)BtlHudDrawLayers }, { .pfn = (void *)BtlHudDrawLayers },
    { .pfn = (void *)BtlHudDrawLayers }, { .pfn = (void *)BtlHudDrawLayers },
    { .pfn = (void *)BtlHudDrawLayers }, { .pfn = (void *)BtlHudDrawLayers },
    { .pfn = (void *)BtlHudDrawLayers }, { .pfn = (void *)BtlHudDrawResult },
};
