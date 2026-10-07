// bdc 0x08810214 ScriptOpCaption
#include "bdc.h"

/* Script opcode handler for the caption line of the battle HUD / talk task (`BtlHud`, core id
   `0x6e`; does nothing if missing). Operands: u16 `mode`, u16 `arg`, inline string `text`. `mode`
   0xfffd: clears the caption duration `hud->hintTimer`; 0xfffe: `UiCaptionSetText``(hud, 1, arg,
   text, -1)` (no timeout); 0xffff: `UiCaptionSetText(hud, 0, 0, NULL, -1)` (hide); any other value:
   `UiCaptionSetText(hud, 1, arg, text, mode)` with `mode` as the duration. Returns 0. */

int ScriptOpCaption(Script *script)
{
    BtlHud *hud;
    s32 mode;
    s32 arg;
    const char *text;

    hud = (BtlHud *)CoreTaskFind(0x6e);
    if (hud == NULL) {
        return 0;
    }
    mode = ScriptReadU16(script);
    arg = ScriptReadU16(script);
    /* ScriptSkipString leaves the string start in v0; read it from the operand pointer */
    text = (const char *)script->operand;
    ScriptSkipString(script);
    if (mode < 0xfffe) {
        if (mode > 0xfffc) {
            hud->hintTimer = 0;
            return 0;
        }
    } else if (mode < 0xffff) {
        UiCaptionSetText(hud, 1, arg, text, -1);
        return 0;
    } else if (mode < 0x10000) {
        UiCaptionSetText(hud, 0, 0, NULL, -1);
        return 0;
    }
    UiCaptionSetText(hud, 1, arg, text, mode);
    return 0;
}
