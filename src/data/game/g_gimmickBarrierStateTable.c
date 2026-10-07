// bdc 0x08a96c10 g_gimmickBarrierStateTable
#include "bdc.h"

__typeof__(VtblEntry[2]) g_gimmickBarrierStateTable = {
    { .fn = (void *)GameGimmickBarrierState00Nop },
    { .fn = (void *)GameGimmickBarrierState01FadeOut },
};
