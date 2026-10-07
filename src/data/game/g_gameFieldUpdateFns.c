// bdc 0x08a91b50 g_gameFieldUpdateFns
#include "bdc.h"

__typeof__(MemberFnPtr[8]) g_gameFieldUpdateFns = {
    { .pfn = (void *)GameFieldPhaseLoad }, { .pfn = (void *)GameFieldPhaseMain },
    { .pfn = (void *)GameFieldPhasePause }, { .pfn = (void *)GameFieldPhaseExit },
    { .pfn = (void *)GameFieldPhaseRepair }, { .pfn = (void *)GameFieldPhaseSettings },
    { .pfn = (void *)GameFieldPhaseScreen380 }, { .pfn = (void *)GameFieldPhaseSave },
};
