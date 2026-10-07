// bdc 0x08af6fd8 g_sndObjectMgrVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_sndObjectMgrVtbl = { {0}, { .fn = (void *)SndObjectMgrDtor } };
