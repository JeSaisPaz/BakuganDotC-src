// bdc 0x08af233c g_coreBezierVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_coreBezierVtbl = { {0}, { .fn = (void *)CoreBezierDtor } };
