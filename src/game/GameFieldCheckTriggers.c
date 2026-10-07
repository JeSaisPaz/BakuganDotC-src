// bdc 0x088c1230 GameFieldCheckTriggers
#include "bdc.h"

/* Event-trigger scan of the field (world map) scene task, task id 500 (`GameFieldCtor`, 0x7a0
   bytes, vtable `0x08af2cfc`) (called by `GameFieldHandleInput`). The player
   (`ActorFindPlayer`) can reach triggers when it exists and is in state 0 or 1; only then is the
   action button (pad pressed 0x4000) read. Walks the active gimmicks of the list `+0x658` and
   classifies each with its virtual slots 10, 12, 13 and 14 (first one that returns non-zero wins):
   - slot 10: contact bit 0 → sets `triggerByContact` (`+0x6f9`) to 1 and returns it; else contact bit 1 and the
     player in front (`GameFieldIsPlayerInFront`, `back` 0) → with the button: `triggerByContact` = 0,
     returns it;
   - slot 12: contact bit 1 → for type ids 0x1774/0x320/0x321/0x323/0x324/0x327 returned only with
     the button, any other type returned at once;
   - slot 13: contact bit 0 → calls its virtual slot 16 with 0 and returns it;
   - slot 14: like slot 10's bit-1 case with `back` 1;
   - none: kind 9, 0x10 or 0x11 with contact bit 1 → returned with the button.
   A candidate that qualifies but waits for the button sets the player's `triggerInReach` when the
   player can reach. Returns the triggered gimmick, or NULL when none fired. */

GameGimmick *GameFieldCheckTriggers(CoreTask *task)

{
  GameFieldTask *field = (GameFieldTask *)task;
  ActorPlayer *player;
  GameGimmick *g;
  const VtblEntry *e;
  bool canReach;
  bool pressed;
  bool inContact;
  u16 type;
  int kind;

  player = (ActorPlayer *)ActorFindPlayer();
  canReach = false;
  if (player != NULL) {
    if (((Actor *)ActorFindPlayer())->state == 0) {
      canReach = true;
    } else if (((Actor *)ActorFindPlayer())->state == 1) {
      canReach = true;
    }
  }
  pressed = false;
  if (g_padState != NULL && canReach) {
    pressed = (g_padState->pressed & 0x4000) != 0;
  }

  for (g = field->gimmicks; g != NULL; g = (GameGimmick *)g->base.base.next) {
    if (g->active == 0) {
      continue;
    }
    if ((e = &((const VtblEntry *)g->base.base.vtable)[10],
         ((int (*)(void *))e->fn)((char *)g + e->delta) != 0)) {
      if (g->contactFlags & 1) {
        field->triggerByContact = 1;
        return g;
      }
      if ((g->contactFlags & 2) == 0) {
        continue;
      }
      if (GameFieldIsPlayerInFront(task, g, false) == 0) {
        continue;
      }
      if (pressed) {
        field->triggerByContact = 0;
        return g;
      }
      if (canReach) {
        player->triggerInReach = 1;
      }
      continue;
    }
    if ((e = &((const VtblEntry *)g->base.base.vtable)[12],
         ((int (*)(void *))e->fn)((char *)g + e->delta) != 0)) {
      inContact = (g->contactFlags & 2) != 0;
      type = g->typeId;
      if (type == 0x1774 || type == 0x320 || type == 0x321 || type == 0x323 || type == 0x324 ||
          type == 0x327) {
        if (!inContact) {
          continue;
        }
        if (pressed) {
          return g;
        }
        if (canReach) {
          player->triggerInReach = 1;
        }
        continue;
      }
      if (inContact) {
        return g;
      }
      continue;
    }
    if ((e = &((const VtblEntry *)g->base.base.vtable)[13],
         ((int (*)(void *))e->fn)((char *)g + e->delta) != 0)) {
      if ((g->contactFlags & 1) == 0) {
        continue;
      }
      e = &((const VtblEntry *)g->base.base.vtable)[16];
      ((void (*)(void *, int))e->fn)((char *)g + e->delta, 0);
      return g;
    }
    if ((e = &((const VtblEntry *)g->base.base.vtable)[14],
         ((int (*)(void *))e->fn)((char *)g + e->delta) != 0)) {
      if ((g->contactFlags & 2) == 0) {
        continue;
      }
      if (GameFieldIsPlayerInFront(task, g, true) == 0) {
        continue;
      }
      if (pressed) {
        field->triggerByContact = 0;
        return g;
      }
      if (canReach) {
        player->triggerInReach = 1;
      }
      continue;
    }
    kind = g->kind;
    if (kind != 9 && kind != 0x10 && kind != 0x11) {
      continue;
    }
    if ((g->contactFlags & 2) == 0) {
      continue;
    }
    if (pressed) {
      return g;
    }
    if (canReach) {
      player->triggerInReach = 1;
    }
  }
  return NULL;
}
