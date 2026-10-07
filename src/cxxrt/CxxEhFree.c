// bdc 0x08a034b8 CxxEhFree
#include "bdc.h"

/* `free` for the exception-object stack. */
void CxxEhFree(void *ptr)
{
    free(ptr);
}
