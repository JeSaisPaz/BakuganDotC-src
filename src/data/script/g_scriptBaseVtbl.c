// bdc 0x08af52a4 g_scriptBaseVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_scriptBaseVtbl = { {0}, { .fn = (void *)ScriptBaseDtor }, { .fn = (void *)ScriptBaseDispatchOpcode } };
