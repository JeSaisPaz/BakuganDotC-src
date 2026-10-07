// bdc 0x089c9ff4 ScriptOpStopTrack
#include "bdc.h"

/* Stops the current track: returns 4, on which `ScriptStep` clears the running bit of
   `curTrack->flags`, so the track never runs again. The script itself lives on while other
   tracks run. */
int ScriptOpStopTrack(Script *script)
{
    (void)script;
    return 4;
}
