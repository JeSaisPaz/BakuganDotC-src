// bdc 0x08a96af8 g_gameGimmickItemBoxStateFns
#include "bdc.h"

__typeof__(MemberFnPtr[3]) g_gameGimmickItemBoxStateFns = {
    { .pfn = (void *)GameGimmickItemBoxState00Nop },
    { .pfn = (void *)GameGimmickItemBoxStateBreak },
    { .pfn = (void *)GameGimmickItemBoxState02Nop },
};
