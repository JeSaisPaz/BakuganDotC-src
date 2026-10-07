// bdc 0x089de85c GfxModelSetName
#include "bdc.h"

/* Copies `name` (truncated to 31 characters) into the model's name field `+0xbd`. */

void GfxModelSetName(GfxModel *self, const char *name)
{
    char buf[0x200];

    strcpy(buf, name);
    buf[31] = 0;
    strcpy(self->name, buf);
}
