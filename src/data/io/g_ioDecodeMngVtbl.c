// bdc 0x08af590c g_ioDecodeMngVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_ioDecodeMngVtbl = { {0}, { .fn = (void *)IoDecodeMngDtor } };
