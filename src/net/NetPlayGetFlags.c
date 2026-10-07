// bdc 0x0881b60c NetPlayGetFlags
#include "bdc.h"

/* Returns the 32-bit flag word (`flags`) of the NetPlay object. `NetPlayLateUpdate` tests bit
   `0x8000000` of it to choose between `NetPlayExchangeEntries` and sending a full `NetCharaMsg`
   input record that carries the word; `NetPlayUpdate` passes it to `NetCharaBothHaveFlags`. */
u32 NetPlayGetFlags(NetPlay *self)
{
    return self->flags;
}
