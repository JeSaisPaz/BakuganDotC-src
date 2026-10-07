// bdc 0x088c2168 GameFieldPartyAdd
#include "bdc.h"

/* Adds party member `member` through the party manager `+0x78c` (`GameFieldCharSetAddActor`, spawns the placed
   actor) and refreshes the `+0x790` manager (`GameEvent470AllocActorRecords`). */

void GameFieldPartyAdd(CoreTask *task, u8 member)

{
  GameFieldTask *field = (GameFieldTask *)task;

  GameFieldCharSetAddActor(field->charSet,member);
  GameEvent470AllocActorRecords(field->events);
  return;
}
