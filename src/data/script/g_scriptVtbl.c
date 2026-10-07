// bdc 0x08af59fc g_scriptVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_scriptVtbl = { {0}, { .fn = (void *)ScriptDtor }, { .fn = (void *)ScriptDispatchOpcode } };
