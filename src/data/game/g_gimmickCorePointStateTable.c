// bdc 0x08a83c54 g_gimmickCorePointStateTable
#include "bdc.h"

__typeof__(VtblEntry[3]) g_gimmickCorePointStateTable = {
    { .fn = (void *)GameGimmickCorePointState00Idle },
    { .fn = (void *)GameGimmickCorePointState01Collected },
    { .fn = (void *)GameGimmickCorePointState02Despawn },
};
