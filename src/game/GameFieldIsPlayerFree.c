// bdc 0x088c1fd0 GameFieldIsPlayerFree
#include "bdc.h"

/* Returns whether the field task (id 500, `GameFieldCtor`) is in its main phase (`phase == 1`)
   with sub-state 5 or 0x15 (`subState`), i.e. the player can move freely. */

bool GameFieldIsPlayerFree(CoreTask *task)
{
  GameFieldTask *field = (GameFieldTask *)task;

  if ((field->subState != 5) && (field->subState != 0x15)) {
    return false;
  }
  return field->phase == 1;
}
