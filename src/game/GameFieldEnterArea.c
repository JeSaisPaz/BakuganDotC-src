// bdc 0x088c0c84 GameFieldEnterArea
#include "bdc.h"

/* Enters the current area (`g_gameEventFlags[0]` area, `g_gameEventFlags[2]` block) of the field task
   (id 500, `GameFieldCtor`): reloads the room's character set (`GameFieldCharSetReload` on
   `charSet`); when `reload` is set, clears the gimmick list and object list `objList670` and reloads the
   object layout (`GameFieldLoadObjects`); resets the gimmicks (`GameFieldResetGimmickStates`) and
   loads the room script into `events` (`GameEvent470LoadScript`). On reload it copies the first of the
   6 `entryPoints` whose id matches `g_gameFieldEntryId` into the 16-byte location block
   `g_gameUnlockFlags` (position, heading; left unchanged when none matches). It then writes that
   location into the player's placement record (slot 0 of `g_gameEventLocationBlock`, rotation
   (0, heading, 0)), places the player actor (`ActorApplyPlacement`), calls its virtual slot 7,
   re-allocates the event actor records (`GameEvent470AllocActorRecords`) and sets `eventViewTimer`
   to 16. */

void GameFieldEnterArea(CoreTask *task, bool reload)

{
  GameFieldTask *field = (GameFieldTask *)task;
  GameFieldPlacedChar **table = (GameFieldPlacedChar **)g_gameEventLocationBlock;
  s32 *locPos = (s32 *)g_gameUnlockFlags;
  s16 *locHeading = (s16 *)&g_gameUnlockFlags[0xc];
  GameFieldPlacedChar *rec;
  GameFieldPlacement *player;
  const GameFieldEntryPoint *ep;
  const VtblEntry *ve;
  s32 i;

  GameFieldCharSetReload(field->charSet, g_gameEventFlags[0], g_gameEventFlags[2], 1);
  if (reload) {
    field->objList670.tail = NULL;
    field->objList670.head = NULL;
    field->objList670.count = 0;
    field->gimmicksTail = NULL;
    field->gimmicks = NULL;
    field->gimmickCount = 0;
    GameFieldLoadObjects(task, g_gameEventFlags[0], g_gameEventFlags[2]);
  }
  GameFieldResetGimmickStates(task, true);
  GameEvent470LoadScript(field->events, 0, 0, 0, (s32)(uintptr_t)&g_gameEventState, 0, /* bdc: ptr-narrow ok: the callee never reads a4 */
                         g_gameEventFlags[0], g_gameEventFlags[2]);
  rec = table[0];
  if (reload) {
    for (i = 0; i < 6; i++) {
      ep = &field->entryPoints[i];
      if (ep->id == g_gameFieldEntryId) {
        locPos[0] = ep->pos[0];
        locPos[1] = ep->pos[1];
        locPos[2] = ep->pos[2];
        *locHeading = ep->heading;
        break;
      }
    }
  }
  rec->pos[0] = locPos[0];
  rec->pos[1] = locPos[1];
  rec->pos[2] = locPos[2];
  rec = table[0];
  rec->rot[0] = 0;
  rec->rot[1] = *locHeading;
  rec->rot[2] = 0;
  ActorApplyPlacement((Actor *)g_gameFieldCharSet->actors[0]);
  player = g_gameFieldCharSet->actors[0];
  ve = &player->vtbl[7];
  ((void (*)(void *))ve->fn)((u8 *)player + ve->delta);
  GameEvent470AllocActorRecords(field->events);
  field->eventViewTimer = 0x10;
  return;
}
