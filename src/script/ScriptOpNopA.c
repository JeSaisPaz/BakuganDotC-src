// bdc 0x089ca1cc ScriptOpNopA
#include "bdc.h"

/* Does nothing and returns 0 (a second no-op handler, distinct from ScriptOpNop). */
int ScriptOpNopA(Script *script)
{
    (void)script;
    return 0;
}
