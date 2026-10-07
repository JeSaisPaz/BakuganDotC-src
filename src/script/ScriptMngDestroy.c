// bdc 0x089cb898 ScriptMngDestroy
#include "bdc.h"

/* Destroys the script manager singleton: calls `ScriptMngShutdown`(g_scriptMng, 3) (destructor +
   free) and clears `g_scriptMng`. No-op when it was never created. */
void ScriptMngDestroy(void)
{
    if (g_scriptMng != NULL) {
        ScriptMngShutdown(g_scriptMng, 3);
        g_scriptMng = NULL;
    }
}
