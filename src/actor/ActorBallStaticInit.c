// bdc 0x088b9224 ActorBallStaticInit
#include "bdc.h"

/* Static initialiser (entry 14 of the static-constructor table `0x08af5bbc`) of the ball module:
   zeroes the `contact` vec4 of the ball hit query `g_actorBallHitQuery` used by `ActorBallCheckHit` and `ActorBallStateThrown`. */

void ActorBallStaticInit(void)

{
  g_actorBallHitQuery.contact.x = 0.0f;
  g_actorBallHitQuery.contact.y = 0.0f;
  g_actorBallHitQuery.contact.z = 0.0f;
  g_actorBallHitQuery.contact.w = 0.0f;
  return;
}
