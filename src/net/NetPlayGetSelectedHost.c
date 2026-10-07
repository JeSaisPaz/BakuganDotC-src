// bdc 0x0881b31c NetPlayGetSelectedHost
#include "bdc.h"

/* Returns the selected-host record (`+0x94`) of the `NetPlay` manager, or NULL when
   none is selected (`+0xb3` clear). See `NetPlaySelectHost`. */

u8 * NetPlayGetSelectedHost(NetPlay *self)

{
  if (self->hasSelectedHost != '\0') {
    return (u8 *)&self->selectedHost;
  }
  return (u8 *)0;
}
