// bdc 0x088efda8 GameEvent470SyncActorRecords
#include "bdc.h"

/* Copies every placed character's position and angles (`g_gameEventLocationBlock[i]`) into all four
   key slots (orig/start/end/current) of its event-actor record, stores the placement pointer as the
   record's actor, and when `i` is the talk character `b` (`+0x2d3`) latches that placement's
   character-set entry into `talkPartner` (`+0x2d4`). The loop runs at least once, then while
   `i < g_gameFieldCharSet->placedCount`. */

void GameEvent470SyncActorRecords(GameEvent470 *self)

{
  GameFieldPlacedChar **table = (GameFieldPlacedChar **)g_gameEventLocationBlock;
  u8 i = 0;

  do {
    GameEventActorRecord *rec = &self->actors[i];
    GameFieldPlacedChar *src = table[i];

    self->actors[i].origPos[0] = src->pos[0];
    self->actors[i].origPos[1] = src->pos[1];
    self->actors[i].origPos[2] = src->pos[2];
    self->actors[i].startPos[0] = src->pos[0];
    self->actors[i].startPos[1] = src->pos[1];
    self->actors[i].startPos[2] = src->pos[2];
    self->actors[i].endPos[0] = src->pos[0];
    self->actors[i].endPos[1] = src->pos[1];
    self->actors[i].endPos[2] = src->pos[2];
    rec->pos[0] = src->pos[0];
    rec->pos[1] = src->pos[1];
    rec->pos[2] = src->pos[2];

    src = table[i];
    rec = &self->actors[i];
    rec->origRot[0] = src->rot[0];
    rec->origRot[1] = src->rot[1];
    rec->origRot[2] = src->rot[2];
    self->actors[i].startRot[0] = src->rot[0];
    self->actors[i].startRot[1] = src->rot[1];
    self->actors[i].startRot[2] = src->rot[2];
    self->actors[i].endRot[0] = src->rot[0];
    self->actors[i].endRot[1] = src->rot[1];
    self->actors[i].endRot[2] = src->rot[2];
    rec->rot[0] = src->rot[0];
    rec->rot[1] = src->rot[1];
    rec->rot[2] = src->rot[2];

    self->actors[i].actor = table[i];
    if (self->talkB == i) {
      self->talkPartner = table[i]->entry;
    }
    i++;
  } while (i < g_gameFieldCharSet->placedCount);
  return;
}
