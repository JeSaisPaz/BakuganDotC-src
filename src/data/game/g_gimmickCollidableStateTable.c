// bdc 0x08a96a00 g_gimmickCollidableStateTable
#include "bdc.h"

__typeof__(VtblEntry[2]) g_gimmickCollidableStateTable = {
    { .fn = (void *)GameGimmickCollidableState00Nop },
    { .fn = (void *)GameGimmickCollidableStateHit },
};
