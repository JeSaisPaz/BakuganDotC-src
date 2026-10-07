// bdc 0x08af5214 g_ioLzsPackageVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_ioLzsPackageVtbl = { {0}, { .fn = (void *)IoLzsPackageDtor } };
