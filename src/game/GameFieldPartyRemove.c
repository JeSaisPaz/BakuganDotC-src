// bdc 0x088c219c GameFieldPartyRemove
#include "bdc.h"

/* Removes party member `member` through the party manager `+0x78c` (`GameFieldCharSetRemoveActor`, unlinks and
   deletes the actor) and refreshes the `+0x790` manager (`GameEvent470AllocActorRecords`). */

void GameFieldPartyRemove(CoreTask *task, u8 member)

{
  GameFieldTask *field = (GameFieldTask *)task;

  GameFieldCharSetRemoveActor(field->charSet,member);
  GameEvent470AllocActorRecords(field->events);
  return;
}
