// bdc 0x08af58ec g_ioDataVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_ioDataVtbl = { {0}, { .fn = (void *)IoDataDtor } };
