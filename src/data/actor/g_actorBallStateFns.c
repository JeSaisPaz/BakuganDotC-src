// bdc 0x08a90898 g_actorBallStateFns
#include "bdc.h"

__typeof__(MemberFnPtr[2]) g_actorBallStateFns = { { .pfn = (void *)ActorBallState00Nop }, { .pfn = (void *)ActorBallStateThrown } };
