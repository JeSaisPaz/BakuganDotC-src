// bdc 0x08ac46dc g_impurePtr
#include "bdc.h"

__typeof__(_reent *) g_impurePtr = (struct _reent *)&g_impureData;
