// bdc 0x089fa280 IoDiscGetError
#include "bdc.h"

/* Returns the error code of the `CODiscSimple` disc reader (`g_discSimple`). */
int IoDiscGetError(IoDiscSimple *self)
{
    return self->error;
}
