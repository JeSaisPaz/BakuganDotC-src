// bdc 0x089c9ab0 ScriptVarsGetProfileOffset
#include "bdc.h"

/* Returns 0x100, the offset of the player-profile data inside the script-variable / save-data block
   (the script globals come first). Used by `SaveProfileAttachBlock`. */

s32 ScriptVarsGetProfileOffset(void)
{
    return 0x100;
}
