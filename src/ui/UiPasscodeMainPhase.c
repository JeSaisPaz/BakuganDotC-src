// bdc 0x0893feec UiPasscodeMainPhase
#include "bdc.h"

/* Main phase (entry 2 of the phase table `0x08a9ce20`) of the sequence-code screen (task 374,
   `UiPasscodeCtor`; the player re-enters a sequence of up to 6 symbols from a 10-symbol pad and
   it is compared with the answer): open animation, optional demonstration of the answer, symbol
   input with the cursor, judgement, close animation; finally reports the result through
   `UiPasscodeSetResult` (UiSetMenuResult 0 = correct, 1 = wrong / gave up) and switches to
   phase 3. */

void UiPasscodeMainPhase(UiScreen *screen)
{
    UiPasscode *ps = (UiPasscode *)screen;
    u8 mode;
    u8 confirm;

    switch (screen->phaseStep) {
    case 0:
        UiPasscodeStartPartTweens(screen, false);
        screen->phaseStep++;
        break;
    case 1:
        if ((u8)UiPasscodePartTweensDone(screen, false) == 1) {
            UiPasscodeShowButtonPrompts(screen, 1);
            mode = ps->mode;
            if (mode == 0) {
                screen->phaseStep++;
            } else if (mode < 2) {
                screen->phaseStep = 0xb;
            } else if (mode < 3) {
                screen->phaseStep = 9;
            }
        }
        break;
    case 2:
        UiPasscodeResetCursor(screen);
        UiPasscodeLayoutEntry(screen);
        screen->phaseStep++;
        break;
    case 3:
        UiPasscodePulseCursor(screen);
        UiPasscodeUpdateCursorPulse(screen);
        confirm = (u8)UiPasscodeCheckConfirm(screen);
        if (confirm == 0) {
            if (UiPasscodeMoveCursor(screen) != 0) {
                if (SndHasManager()) {
                    SndManagerPlay(SndGetManager(), 0x2c0001c, 0, 0);
                }
                UiPasscodeResetCursor(screen);
            } else if ((screen->pad->pressed & 0x2000) != 0) {
                if (SndHasManager()) {
                    SndManagerPlay(SndGetManager(), 2, 0, 0);
                }
                if (ps->entryCount == 0) {
                    UiPasscodeHideCursor(ps);
                    ps->judgement = 1;
                    screen->phaseStep++;
                } else {
                    UiPasscodeRemoveSymbol(ps);
                    UiPasscodeLayoutEntry(screen);
                }
            }
        } else if (confirm < 2) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x2c0001e, 0, 0);
            }
            UiPasscodeResetCursor(screen);
            UiPasscodeStartPressFlash(screen);
            screen->phaseStep = 6;
        } else if (confirm < 3) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 3, 0, 0);
            }
        }
        break;
    case 4:
        UiPasscodeStartPartTweens(screen, true);
        screen->phaseStep++;
        break;
    case 5:
        if ((u8)UiPasscodePartTweensDone(screen, true) == 1) {
            screen->phaseStep = 0xd;
        }
        break;
    case 6:
        if (UiPasscodePressFlashDone() != 0) {
            if (ps->onCommandRow == 0) {
                UiPasscodeAddSymbol(ps);
                UiPasscodeResetCursor(screen);
                UiPasscodeLayoutEntry(screen);
                screen->phaseStep = 3;
            } else if ((&ps->focusSymbol)[ps->onCommandRow] == 0) {
                /* byte 0x75 + onCommandRow: `command` on the command row; command 0 = erase */
                UiPasscodeRemoveSymbol(ps);
                screen->phaseStep = 2;
            } else {
                screen->phaseStep = 7;
            }
        }
        break;
    case 7:
        ps->judgement = UiPasscodeCheckEntry(screen) != 1;
        UiPasscodeResetPlayback(ps);
        screen->phaseStep++;
        break;
    case 8:
        if (UiPasscodeShowJudgement(screen) == 1) {
            UiPasscodeHideCursor(ps);
            UiPasscodeShowButtonPrompts(screen, 0);
            screen->phaseStep = 4;
        }
        break;
    case 9:
        UiPasscodeResetPlayback(ps);
        screen->phaseStep++;
        break;
    case 10:
        if (UiPasscodePlaySequence(screen) == 1) {
            screen->phaseStep = 2;
        }
        break;
    case 0xb:
        UiPasscodeResetPlayback(ps);
        screen->phaseStep++;
        break;
    case 0xc:
        if (UiPasscodeShowSequence(screen) == 1) {
            screen->phaseStep = 2;
        }
        break;
    default:
        UiPasscodeSetResult(ps);
        screen->phase = 3;
        screen->phaseStep = 0;
        break;
    }
}
