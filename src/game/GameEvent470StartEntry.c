// bdc 0x088eff5c GameEvent470StartEntry
#include "bdc.h"

/* Starts script entry `entry` (offset table `+0x18`: `{u16 first, u16 count}` into the commands
   `+0x1c`) for the talk between characters `a` (`talkA`) and `b` (`talkB`): resets the transition
   block `g_gameEventTransitionArg` from its defaults, removes pending actor actions (kind 4),
   syncs the actor records, sets actor `a`'s `talkPartner` to actor `b`, copies the turn defaults
   to `turnPending`.. and starts the commands (`GameEventStartScript`). */

void GameEvent470StartEntry(GameEvent470 *self, s16 entry, u8 a, u8 b)
{
  GameEventScriptEntry *entryRec;
  GameEventCommand *cmds;

  entryRec = &self->base.entries[entry];
  cmds = &self->base.commands[entryRec->first];
  memcpy(&g_gameEventTransitionArg, g_gameEvent470TransitionDefaults, 8);
  self->talkA = a;
  self->talkB = b;
  GameEventActionListRemoveKind(self->base.actions, 4);
  GameEvent470SyncActorRecords(self);
  ((Actor *)g_gameFieldCharSet->actors[a])->talkPartner = (Actor *)g_gameFieldCharSet->actors[b];
  memcpy(&self->turnPending, g_gameEvent470TurnDefaults, 0x10);
  GameEventStartScript(&self->base, cmds, entryRec->count);
}
