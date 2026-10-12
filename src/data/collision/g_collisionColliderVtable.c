// bdc 0x08af1684 g_collisionColliderVtable
#include "bdc.h"

__typeof__(VtblEntry[2]) g_collisionColliderVtable = { {0}, { .fn = (void *)CollisionColliderDtor } };
