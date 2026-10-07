// bdc 0x088d73a8 GameGimmickItemBoxUpdate
#include "bdc.h"

/* Update (vtable slot 7) of the item box (breakable obstacle) gimmick (`GameGimmickItemBoxCtor`,
   vtables `g_gameGimmickItemBoxVtbl`/`g_gameGimmickItemBoxVtbl2`): `GameGimmickUpdate`,
   forces full alpha (`ambient[3]`) while the player is in state 10, then, unless the alpha is
   `<= 0`, runs the state handler (virtual slot 20) for states 0..2, animates the model, updates the
   prompt (`GameGimmickItemBoxUpdatePrompt`) and, once per frame for all boxes (guard
   `g_itemBoxEffectUpdated`, cleared by `GameGimmickItemBoxDraw`), animates the shared
   break-effect models. */

void GameGimmickItemBoxUpdate(GameGimmickItemBox *obj)

{
  Actor *player;
  u32 state;

  GameGimmickUpdate(&obj->base);
  player = (Actor *)ActorFindPlayer();
  if (player->state == 10) {
    (obj->base).base.ambient[3] = 1.0f;
  }
  if (!((obj->base).base.ambient[3] <= 0.0f)) {
    state = (u32)(obj->base).state;
    if (((s32)state >= 0) && (state < 3)) {
      const VtblEntry *e = &((const VtblEntry *)(obj->base).base.base.vtable)[20];

      ((void (*)(void *))e->fn)((u8 *)obj + e->delta);
    }
    GfxModelUpdateMotion(&(obj->base).base);
    GfxModelApplyMotion(&(obj->base).base);
    GameGimmickItemBoxUpdatePrompt(obj);
    if (g_itemBoxEffectUpdated == 0) {
      if (g_itemBoxBreakEffect1 != (GfxModel *)0x0) {
        GfxModelUpdateMotion(g_itemBoxBreakEffect1);
        GfxModelApplyMotion(g_itemBoxBreakEffect1);
      }
      if (g_itemBoxBreakEffect2 != (GfxModel *)0x0) {
        GfxModelUpdateMotion(g_itemBoxBreakEffect2);
        GfxModelApplyMotion(g_itemBoxBreakEffect2);
      }
      g_itemBoxEffectUpdated = g_itemBoxEffectUpdated + 1;
    }
  }
  return;
}
