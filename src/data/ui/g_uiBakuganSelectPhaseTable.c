// bdc 0x08a9c340 g_uiBakuganSelectPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiBakuganSelectPhaseTable = {
    { .pfn = (void *)UiBakuganSelectPhaseLoad }, { .pfn = (void *)UiBakuganSelectPhaseWait },
    { .pfn = (void *)UiBakuganSelectMainPhase }, { .pfn = (void *)UiBakuganSelectPhaseClose },
};
