// bdc 0x088df9dc ActorSetList
#include "bdc.h"

/* Installs `list` as the global actor list `g_actorList` and resets the actor numbering
   (`g_actorNumbering` = 0, `g_actorSerial` = 1). Called when a field or battle demo loads. */

void ActorSetList(void *list)

{
  g_actorNumbering = 0;
  g_actorSerial = 1;
  g_actorList = list;
  return;
}
