// bdc 0x088ef6a4 GameEventEnd
#include "bdc.h"

/* Virtual slot 8 of the field event task base class: calls virtual slot 7 through the vtable and
   sets the event state byte `+0x264` to 1 (idle). Overridden in task 470. */

void GameEventEnd(GameEvent *self)

{
  const VtblEntry *entry;

  entry = &((const VtblEntry *)(self->base).vtable)[7];
  ((void (*)(void *))entry->fn)((char *)self + entry->delta);
  self->state = '\x01';
  return;
}

