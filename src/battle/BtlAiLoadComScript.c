// bdc 0x0889ad74 BtlAiLoadComScript
#include "bdc.h"

/* Loads the CPU AI rule script `ComScript%02d.bin` (`id`) from the loaded pack chain
   (`CorePackChainFind` on `g_ioLzsPackages`) into `rules` (`BtlAiComScript`): copies the
   8-byte file header (five bytes to `header`, s16 `groupCount`, byte `headerByte7`), allocates
   the four-entry `BtlAiRuleGroup` table `groups` from the low heap, and for each group below
   `groupCount` reads its record count and allocates a `CxxVecNew` array of `BtlAiRuleRecord`s
   (constructed by `BtlAiRuleRecordCtor`, cookie `g_cxxVecCookieSize`) filled from the packed
   45-byte file records; unused groups get `{0, NULL}`. The file is read byte-wise (multi-byte
   fields are unaligned), so every multi-byte field is a `memcpy`. No NULL checks on the file or
   the group table. */

static inline void *BtlAiComScriptAllocLow(s32 size)
{
    bool fromLow;
    void *block;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    block = MemAlloc(size, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    return block;
}

void BtlAiLoadComScript(void *rules, s32 id)
{
    BtlAiComScript *script = rules;
    char name[40];
    const u8 *src;
    s32 g;

    sprintf(name, "ComScript%02d.bin", id);
    src = CorePackChainFind(g_ioLzsPackages, name);
    script->header[0] = src[0];
    script->header[1] = src[1];
    script->header[2] = src[2];
    script->header[3] = src[3];
    script->header[4] = src[4];
    memcpy(&script->groupCount, src + 5, sizeof(s16));
    script->headerByte7 = src[7];
    src += 8;
    script->groups = BtlAiComScriptAllocLow(4 * sizeof(BtlAiRuleGroup));

    for (g = 0; g < 4; g++) {
        BtlAiRuleGroup *group = &script->groups[g];
        s32 count;
        void *block;
        BtlAiRuleRecord *records;
        s32 r;

        if (g >= script->groupCount) {
            group->count = 0;
            script->groups[g].records = NULL;
            continue;
        }
        memcpy(&group->count, src, sizeof(s32));
        src += 4;
        count = script->groups[g].count;
        records = NULL;
        block = BtlAiComScriptAllocLow(count * (s32)sizeof(BtlAiRuleRecord) + 0x10 /* PSP: CxxVecNew array cookie */);
        if (block != NULL) {
            records = CxxVecNew((u8 *)block + g_cxxVecCookieSize, count,
                                sizeof(BtlAiRuleRecord), BtlAiRuleRecordCtor, 0);
        }
        script->groups[g].records = records;

        for (r = 0; r < script->groups[g].count; r++) {
            BtlAiRuleRecord *rec = &script->groups[g].records[r];
            u8 op;
            s32 c;

            memcpy(&rec->command, src, sizeof(s16));
            rec->byte02 = src[2];
            rec->byte03 = src[3];
            memcpy(&rec->weightMin, src + 4, sizeof(u16));
            memcpy(&rec->weightMax, src + 6, sizeof(u16));
            rec->minLevel = src[8];
            rec->maxLevel = src[9];
            op = src[10];
            src += 11;
            for (c = 0; c < 3; c++) {
                rec->cond[c].op = op;
                memcpy(&rec->cond[c].value, src, sizeof(float));
                rec->cond[c].arg = src[4];
                op = src[5];
                src += 6;
            }
            rec->byte30 = op;
            memcpy(&rec->float34, src, sizeof(float));
            memcpy(&rec->float38, src + 4, sizeof(float));
            rec->byte3c = src[8];
            rec->byte3d = src[9];
            memcpy(&rec->short40, src + 10, sizeof(s16));
            memcpy(&rec->modeMask, src + 12, sizeof(u32));
            src += 16;
        }
    }
}
