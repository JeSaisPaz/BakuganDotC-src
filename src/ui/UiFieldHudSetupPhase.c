// bdc 0x088d04ac UiFieldHudSetupPhase
#include "bdc.h"

/* Phase 1 of the field HUD. Does nothing while there is no actor list (`ActorGetList`) or the
   list is empty. Otherwise stores the first actor in `setupState`, counts the target gimmicks
   (`UiFieldHudCountTargets`), stores the player (`ActorFindPlayer`) and walks the actor list:
   every actor adds one icon, NPC models 0x4e..0x53 a second one, and model 0x53 also adds one
   to `targetCount`. The icon count minus one goes to both words of `iconBase`. Then allocates
   the sprite table `base.data` (`iconBase + targetCount + 0x48` pointers, low heap; only the
   first word cleared), creates the 12 layout sprites (`UiLayoutCreateSprites`) and the guide
   icons (`UiFieldHudBuildGuideIcons`), initialises the fader (`UiFieldHudFaderInit`),
   copies the profile's points into `shownPoints`, loads the hints (`UiFieldHudLoadHints`)
   and goes to phase 2. */

void UiFieldHudSetupPhase(UiFieldHud *self)
{
  bool fromLow;
  Actor *actor;
  s32 *iconCount;
  void *sprites;
  void *spriteTable;

  if (ActorGetList() == NULL) {
    return;
  }
  actor = *(Actor **)ActorGetList();
  *(Actor **)self->setupState = actor;
  if (actor == NULL) {
    return;
  }
  UiFieldHudCountTargets(self);
  self->player = ActorFindPlayer();
  iconCount = (s32 *)self->iconBase;
  for (actor = *(Actor **)self->setupState; actor != NULL;
       actor = (Actor *)actor->base.base.next) {
    iconCount[0]++;
    if (actor->base.base.unk08 >= 0x4e && actor->base.base.unk08 < 0x54) {
      iconCount[0]++;
      if (actor->base.base.unk08 == 0x53) {
        self->targetCount++;
      }
    }
  }
  iconCount[0] = iconCount[0] - 1;
  iconCount[1] = iconCount[0];
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  spriteTable = MemAlloc((iconCount[0] + self->targetCount + 0x48) * 4, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->base.data = spriteTable;
  memset(spriteTable, 0, 4);
  UiLayoutCreateSprites(self->base.spriteLayer, self->base.data, 0xc);
  UiFieldHudBuildGuideIcons(self);
  sprites = self->base.data;
  UiFieldHudFaderInit(&self->fader, &sprites);
  self->shownPoints = SaveGetProfile()->data->points;
  UiFieldHudLoadHints(self);
  self->base.phase = 2;
}
