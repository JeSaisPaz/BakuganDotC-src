// bdc 0x089fd81c IoHasPacLoader
#include "bdc.h"

/* Returns whether `g_ioPacLoader` is non-null (singleton accessor, named by `bdc singleton`). */
bool IoHasPacLoader(void)
{
    return g_ioPacLoader != NULL;
}
