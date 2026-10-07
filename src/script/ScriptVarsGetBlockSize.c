// bdc 0x089c9aa8 ScriptVarsGetBlockSize
#include "bdc.h"

/* Returns 0xfd0, the size of the combined script-variable / save-data block (0x100 bytes of script
   globals + the 0xed0-byte profile block). Wrapped by SaveGetDataBlockSize. */
s32 ScriptVarsGetBlockSize(void)
{
    return 0xfd0;
}
