// bdc 0x08ac616c g_feedbackTextureDtorRecord
#include "bdc.h"

__typeof__(CxxGlobalRecord) g_feedbackTextureDtorRecord = { .object = (void *)&g_feedbackTexture, .destructor = (void (*)(void *))GfxTextureDtor };
