// bdc 0x0894fbdc UiNetMenuMainPhase
#include "bdc.h"

/* Main phase (entry 2 of the phase table `0x08a9d360`) of the network-play menu (task 1999,
   `UiNetMenuCtor`; host-or-join choice that creates the lobby task 2000,
   `UiNetLobbyHostPhase`/`UiNetLobbyJoinPhase`): opens buttons/panels, lets the player choose
   host or join (`+0x74`), on confirm creates the lobby (`CoreTaskCreate``(2000, 100)`, then sets
   its field 5 to the choice through vtable slot 5) and waits until it is gone; the lobby's result
   (`UiGetMenuResult`, menu result) either returns to the choice (0, also
   `NetSetLocalPlayerIndex``(-1)`) or closes the menu (1..2). Cancel (pad `buttons` 0x2000, sound
   2; `cancelled` = 1) closes it; a disabled choice plays sound 3. */

void UiNetMenuMainPhase(UiScreen *screen)

{
  UiNetMenu *menu = (UiNetMenu *)screen;
  CoreTask *lobby;
  const VtblEntry *slot;
  s32 result;
  u8 confirm;
  u8 done;

  switch (screen->phaseStep) {
  case 0:
    UiNetMenuStartButtonTween(screen, false);
    screen->phaseStep = screen->phaseStep + 1;
    break;
  case 1:
    if (UiNetMenuUpdateButtonTween(screen, false)) {
      UiNetMenuStartButtonSlide(screen, 0);
      UiNetMenuBeginTitleFade(screen, 0);
      UiNetMenuBeginHelpWindowFade(screen, 0);
      screen->phaseStep = screen->phaseStep + 1;
    }
    break;
  case 2:
    done = UiNetMenuUpdatePanelTween(screen, false);
    done = (u8)(done + UiNetMenuUpdateTitleFade(screen, 0));
    done = (u8)(done + UiNetMenuUpdateHelpWindowFade(screen, 0));
    if (done == 3) {
      UiNetMenuResetButtonGlow(screen, 1, (u8)menu->choice);
      screen->phaseStep = screen->phaseStep + 1;
    }
    break;
  case 3:
    UiNetMenuPulseButtonGlow(screen, (u8)menu->choice);
    confirm = (u8)UiNetMenuCheckConfirm(screen);
    if (confirm == 0) {
      if ((screen->pad->buttons & 0x2000) != 0) {
        /* cancel */
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 2, 0, 0);
        }
        menu->cancelled = 1;
        screen->phaseStep = 5;
        return;
      }
      if (UiNetMenuMoveCursor(screen) == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        UiNetMenuStartCursorMove(screen);
        screen->phaseStep = 4;
      }
    }
    else if (confirm == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiNetMenuStartConfirmFlash(screen);
      menu->cancelled = 0;
      screen->phaseStep = 7;
      return;
    }
    else {
      /* chosen button disabled */
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 3, 0, 0);
      }
    }
    break;
  case 4:
    if (UiNetMenuAnimateCursorMove(screen) == 1) {
      UiNetMenuResetButtonGlow(screen, 1, (u8)menu->choice);
      screen->phaseStep = 3;
    }
    break;
  case 5:
    UiNetMenuStartButtonTween(screen, true);
    UiNetMenuStartButtonSlide(screen, 1);
    UiNetMenuBeginTitleFade(screen, 1);
    UiNetMenuBeginHelpWindowFade(screen, 1);
    screen->phaseStep = screen->phaseStep + 1;
    break;
  case 6:
    done = UiNetMenuUpdateButtonTween(screen, true);
    done = (u8)(done + UiNetMenuUpdatePanelTween(screen, true));
    done = (u8)(done + UiNetMenuUpdateTitleFade(screen, 1));
    done = (u8)(done + UiNetMenuUpdateHelpWindowFade(screen, 1));
    if (done == 4) {
      screen->phaseStep = 9;
    }
    break;
  case 7:
    if (UiNetMenuConfirmFlashDone() == 1) {
      CoreTaskCreate(2000, 100);
      lobby = (CoreTask *)CoreTaskFind(2000);
      if (lobby != NULL) {
        /* vtable slot 5: set field 5 of the lobby to the choice */
        slot = &((const VtblEntry *)lobby->vtable)[5];
        ((void (*)(void *, s32, s32))slot->fn)((u8 *)lobby + slot->delta, 5, menu->choice);
        screen->phaseStep = 8;
        UiNetMenuSetHelpText(screen, (u8)(menu->choice + 2));
      }
    }
    break;
  case 8:
    if (CoreTaskExists(2000) == 0) {
      result = UiGetMenuResult(screen);
      if (result > 0) {
        if (result < 3) {
          screen->phaseStep = 5;
        }
      }
      else if (result >= 0) {
        screen->phaseStep = 3;
        UiNetMenuSetHelpText(screen, (u8)menu->choice);
        NetSetLocalPlayerIndex(-1);
      }
    }
    break;
  default:
    UiNetMenuSetResult(screen);
    screen->phaseStep = 0;
    screen->phase = screen->phase + 1;
    break;
  }
}
