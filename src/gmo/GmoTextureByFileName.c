// bdc 0x089daa18 GmoTextureByFileName
#include "bdc.h"

/* Copies `fileName` to a 256-byte stack buffer, cuts it at the first `.` (drops the extension) and
   returns `GfxFindTexture` of that base name plus `0x60`, i.e. the address of the texture's
   `GmoTexture` record `gmo`; 0 if the lookup returned NULL (it never does, see `GfxFindTexture`). */
void *GmoTextureByFileName(const char *fileName)
{
    char buf[256];
    void *result = NULL;
    void *found;
    int i;

    strcpy(buf, fileName);
    for (i = 0; buf[i] != '.'; i++) {
        if (buf[i] == '\0') {
            goto lookup;
        }
    }
    buf[i] = '\0';
lookup:
    found = GfxFindTexture(buf);
    if (found != NULL) {
        result = &((GfxTexture *)found)->gmo;
    }
    return result;
}
