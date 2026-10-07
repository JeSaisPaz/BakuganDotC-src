// bdc 0x089ca760 ScriptOpEndScript
#include "bdc.h"

/* Ends the whole script: returns 5, on which `ScriptStep` stops the current track and returns
   -1, which makes `ScriptMngUpdate` destroy the script. */

int ScriptOpEndScript(Script *script)
{
    (void)script;
    return 5;
}
