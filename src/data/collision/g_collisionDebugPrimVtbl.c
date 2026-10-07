// bdc 0x08af581c g_collisionDebugPrimVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_collisionDebugPrimVtbl = { {0}, { .fn = (void *)CollisionDebugPrimDtor } };
