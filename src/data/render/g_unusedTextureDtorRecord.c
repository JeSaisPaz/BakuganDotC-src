// bdc 0x08aac99c g_unusedTextureDtorRecord
#include "bdc.h"

__typeof__(CxxGlobalRecord) g_unusedTextureDtorRecord = { .object = (void *)&g_unusedTexture, .destructor = (void (*)(void *))GfxTextureDtor };
