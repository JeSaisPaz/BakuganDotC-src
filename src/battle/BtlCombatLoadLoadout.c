// bdc 0x088867e0 BtlCombatLoadLoadout
#include "bdc.h"

/* Loads a unit's special-art loadout and owned upgrades from the save profile into its
   `BtlCombatState`. Clears `artSlotCount`; returns when the combat state has no owner. Reads the
   owner's art pair from the profile word table (words `0x36 + playerSlot * 2` and `+1`, 0 when the
   table is missing, 0xff = none) and swaps the two words in the profile when both are set and the
   first is larger. Kinds outside 1..0x14 stop here. In rule mode 1 (script variable 8) for an owner
   whose vtable slot 13 predicate holds (CPU unit), except on stage 0x24 (script variable 1), it
   overwrites the art row of slot 1 (words 0x38/0x39/0x3a) with kind-dependent forced art ids
   (`kind*4 - 4`, `kind*4 - 1/-2/-3`, -1; some depend on stages 8..0xb) and reads that row instead;
   every equipped art then starts charged. The chosen pair goes to `artIds[0..1]` (0xff → -1,
   compacted into slot 0). In rule mode 1 for an owner whose predicate is false it fills
   `slotIds[0..5]` with `UiUpgradeGetUpgradeId``(kind, slot)` for every upgrade slot the profile
   owns (`upgradeOwned[kind][slot]`), 0 otherwise. Then `BtlCombatResetArtSlots`; in the
   forced-art case every equipped art's charge is set to 5000.0. */

/* Profile word `index`, 0 when the word table is missing. */
static s32 LoadoutGetWord(s32 index)
{
    SaveProfile *profile = SaveGetProfile();

    if (profile->words == NULL) {
        return 0;
    }
    return (s32)profile->words[index];
}

/* Stores profile word `index` unless the word table is missing. */
static void LoadoutSetWord(s32 index, s32 value)
{
    SaveProfile *profile = SaveGetProfile();

    if (profile->words != NULL) {
        profile->words[index] = (u32)value;
    }
}

/* Calls the unit's vtable slot 13 predicate (true for a CPU unit). */
static s32 LoadoutOwnerIsCpu(BtlBakugan *owner)
{
    const VtblEntry *entry = &((const VtblEntry *)owner->base.base.vtable)[13];

    return ((s32 (*)(void *))entry->fn)((u8 *)owner + entry->delta);
}

void BtlCombatLoadLoadout(BtlCombatState *combat)
{
    BtlBakugan *owner = combat->owner;
    s32 kind;
    s32 row;
    s32 first;
    s32 second;
    s32 art;
    s32 i;
    bool forced = false;

    combat->artSlotCount = 0;
    if (owner == NULL) {
        return;
    }
    kind = (s32)owner->base.base.unk08;
    row = owner->playerSlot;

    first = LoadoutGetWord(0x36 + row * 2);
    second = LoadoutGetWord(0x37 + row * 2);
    if (first != 0xff && second != 0xff && first > second) {
        LoadoutSetWord(0x36 + row * 2, second);
        LoadoutSetWord(0x37 + row * 2, first);
    }

    owner = combat->owner;
    if (owner == NULL) {
        return;
    }
    if (kind < 1 || kind > 0x14) {
        return;
    }

    if (g_scriptGlobalVars[8] == 1 && LoadoutOwnerIsCpu(owner) != 0) {
        s32 stage = g_scriptGlobalVars[1];

        if (stage != 0x24) {
            s32 base = kind * 4;
            bool earlyStage = stage == 8 || stage == 9 || stage == 10 || stage == 0xb;

            row = 1;
            LoadoutSetWord(0x38, base - 4);
            LoadoutSetWord(0x39, base - 1);
            LoadoutSetWord(0x3a, -1);
            switch (kind) {
            case 1:
            case 7:
            case 9:
            case 10:
            case 0xb:
                LoadoutSetWord(0x39, base - 3);
                break;
            case 0xc:
                LoadoutSetWord(0x39, earlyStage ? base - 3 : base - 1);
                break;
            case 0xd:
                LoadoutSetWord(0x39, earlyStage ? base - 3 : base - 2);
                break;
            case 0xe:
                LoadoutSetWord(0x39, earlyStage ? base - 2 : base - 1);
                break;
            default:
                break;
            }
            forced = true;
        }
    }

    art = LoadoutGetWord(0x36 + row * 2);
    if (art == 0xff) {
        art = -1;
    }
    combat->artIds[0] = art;
    art = LoadoutGetWord(0x37 + row * 2);
    if (art == 0xff) {
        art = -1;
    }
    combat->artIds[1] = art;
    if (combat->artIds[0] == -1 && combat->artIds[1] != -1) {
        combat->artIds[0] = combat->artIds[1];
        combat->artIds[1] = -1;
    }

    if (g_scriptGlobalVars[8] == 1 && LoadoutOwnerIsCpu((BtlBakugan *)combat->owner) == 0) {
        for (i = 0; i < 6; i++) {
            SaveProfile *profile = SaveGetProfile();
            s32 id = 0;

            if (profile->data->upgradeOwned[kind][i] != 0) {
                id = UiUpgradeGetUpgradeId(kind, i);
            }
            combat->slotIds[i] = id;
        }
    }

    BtlCombatResetArtSlots(combat);
    if (forced) {
        for (i = 0; i < 3; i++) {
            if (combat->artIds[i] != -1) {
                combat->artCharge[i] = 5000.0f;
            }
        }
    }
}
