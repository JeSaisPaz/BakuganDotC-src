// bdc 0x08af2d34 g_uiTalkBalloonSpriteVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_uiTalkBalloonSpriteVtbl = { {0}, { .fn = (void *)UiTalkBalloonSpriteDtor }, { .fn = (void *)UiTalkBalloonSpriteUpdate } };
