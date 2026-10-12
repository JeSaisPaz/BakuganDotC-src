// bdc 0x08af525c g_sndObjectVtable
#include "bdc.h"

__typeof__(VtblEntry[2]) g_sndObjectVtable = { {0}, { .fn = (void *)SndObjectDestroy } };
