// bdc 0x08af54ec g_collisionPhysBoxVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_collisionPhysBoxVtbl = { {0}, { .fn = (void *)CollisionPhysBoxDtor }, { .fn = (void *)CollisionPhysBoxStep } };
