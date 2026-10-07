// bdc 0x088ea530 GameEventPropDelete
#include "bdc.h"

/* Deletes a prop (`GameEventPropDtor` with flags 3) when not NULL. */

void GameEventPropDelete(void *prop)

{
  if (prop != (void *)0x0) {
    GameEventPropDtor(prop,3);
  }
  return;
}

