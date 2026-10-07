// bdc 0x08a91b90 g_gameFieldDrawFns
#include "bdc.h"

__typeof__(MemberFnPtr[8]) g_gameFieldDrawFns = {
    { .pfn = (void *)GameFieldDrawLoading }, { .pfn = (void *)GameFieldDrawScene },
    { .pfn = (void *)GameFieldDrawScene }, { .pfn = (void *)GameFieldDrawScene },
    { .pfn = (void *)GameFieldDrawScene }, { .pfn = (void *)GameFieldDrawSettingsNop },
    { .pfn = (void *)GameFieldDrawScene }, { .pfn = (void *)GameFieldDrawScene },
};
