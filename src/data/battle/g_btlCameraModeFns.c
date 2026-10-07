// bdc 0x08aba75c g_btlCameraModeFns
#include "bdc.h"

__typeof__(MemberFnPtr[3]) g_btlCameraModeFns = {
    { .pfn = (void *)BtlCameraUpdateDefault }, { .pfn = (void *)BtlCameraUpdateLockOn },
    { .pfn = (void *)BtlCameraUpdateFollow },
};
