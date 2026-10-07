// bdc 0x0881158c ScriptOpResetProfile
#include "bdc.h"

/* Script opcode (no operands) that resets the player profile to its defaults: when a profile exists
   (`SaveHasProfile`) it runs `SaveProfileReset` and `SaveProfileResetWordTable``(profile,
   1)`. Nothing is written to the memory stick. Returns 0. */

int ScriptOpResetProfile(Script *script)
{
    if (SaveHasProfile()) {
        SaveProfileReset(SaveGetProfile());
        SaveProfileResetWordTable(SaveGetProfile(), true);
    }
    return 0;
}
