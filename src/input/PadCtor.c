// bdc 0x089ce51c PadCtor
#include "bdc.h"

/* Constructor of the main controller object (`PadState`, 0x5c bytes): runs `PadBaseCtor`, sets
   the two ids at `+0x50` and `+0x54` to -1 and `flag58` (`+0x58`) to 0, then `PadInitAndPoll`.
   Returns `pad`. */

PadState *PadCtor(PadState *pad)

{
  PadBaseCtor(pad);
  pad->id50 = -1;
  pad->id54 = -1;
  pad->flag58 = '\0';
  PadInitAndPoll(pad);
  return pad;
}

