// bdc 0x089d7ce8 CoreDebugNop
#include "bdc.h"

/* Empty function: a debug hook compiled out of this build. Called from `MemAlloc`/`MemFree`
   (debug tagging), `BtlCameraUpdate` and `ScriptOpDebug` (where it would set a `host0:` path). */
void CoreDebugNop(void)
{
}
