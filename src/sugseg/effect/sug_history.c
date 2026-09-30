#include "common.h"
#include "game.h"
#include "dcb/sug_history.h"
#include "dcb/heap.h"

typedef struct {
    s32 *words;
    s16 *halves;
    s32 head;
    s32 last;
    s32 count;
    s32 rows;
} ValueHistory;

/* not referenced by any code */
const s32 D_801DDF38 = 4;

PosHistory *SUG_createPosHistory(s32 columns, s32 rows) {
    PosHistory *table;
    s32 count;
    s32 i;

    table = allocTaskHeapBlock(sizeof(PosHistory));
    count = columns * rows;
    table->quads = allocTaskHeapBlock(count * sizeof(Quad));
    table->shorts = allocTaskHeapBlock(count * sizeof(Short4));
    table->count = count;
    table->rows = rows;
    table->head = 0;
    table->last = count - 1;
    for (i = 0; i < count; i++) {
        table->quads[i].v[0] = 0;
        table->quads[i].v[1] = 0;
        table->quads[i].v[2] = 0;
        table->shorts[i].v[0] = 0;
        table->shorts[i].v[1] = 0;
        table->shorts[i].v[2] = 0;
    }
    return table;
}

void SUG_fillPosHistory(PosHistory *table, Quad *quad, Short4 *s4) {
    s32 i;

    for (i = 0; i < table->count; i++) {
        table->quads[i] = *quad;
        table->shorts[i] = *s4;
    }
}

void SUG_pushPosHistory(PosHistory *table, Quad *quad, Short4 *s4) {
    if (quad != NULL) {
        table->quads[table->head] = *quad;
    }
    if (s4 != NULL) {
        table->shorts[table->head] = *s4;
    }
    table->last = table->head;
    table->head = (table->head + 1) % table->count;
}

void SUG_getPosHistory(PosHistory *table, s32 back, Quad *quad, Short4 *s4) {
    s32 i;

    if (back == 0) {
        if (quad != NULL) {
            *quad = table->quads[table->last];
        }
        if (s4 != NULL) {
            *s4 = table->shorts[table->last];
        }
    } else {
        i = table->head;
        i -= back * table->rows;
        if (i < 0) {
            i += table->count;
        }
        if (quad != NULL) {
            *quad = table->quads[i];
        }
        if (s4 != NULL) {
            *s4 = table->shorts[i];
        }
    }
}

void SUG_freePosHistory(void **obj) {
    obj[0] = (void *)freeHeapBlock(obj[0]);
    obj[1] = (void *)freeHeapBlock(obj[1]);
    freeHeapBlock(obj);
}

ValueHistory *SUG_createValueHistory(s32 columns, s32 rows) {
    ValueHistory *table;
    s32 count;
    s32 i;

    table = allocTaskHeapBlock(sizeof(ValueHistory));
    count = columns * rows;
    table->words = allocTaskHeapBlock(count * 4);
    table->halves = allocTaskHeapBlock(count * 2);
    table->count = count;
    table->rows = rows;
    table->head = 0;
    table->last = count - 1;
    for (i = 0; i < count; i++) {
        table->words[i] = 0;
        table->halves[i] = 0;
    }
    return table;
}

void SUG_fillValueHistory(ValueHistory *table, s32 word, s16 half) {
    s32 i;

    for (i = 0; i < table->count; i++) {
        table->words[i] = word;
        table->halves[i] = half;
    }
}

void SUG_pushValueHistory(ValueHistory *table, s32 *word, s16 *half) {
    if (word != NULL) {
        table->words[table->head] = *word;
    }
    if (half != NULL) {
        table->halves[table->head] = *half;
    }
    table->last = table->head;
    table->head = (table->head + 1) % table->count;
}

void SUG_getValueHistory(ValueHistory *table, s32 back, s32 *word, s16 *half) {
    s32 i;

    if (back == 0) {
        if (word != NULL) {
            *word = table->words[table->last];
        }
        if (half != NULL) {
            *half = table->halves[table->last];
        }
    } else {
        i = table->head;
        i -= back * table->rows;
        if (i < 0) {
            i += table->count;
        }
        if (word != NULL) {
            *word = table->words[i];
        }
        if (half != NULL) {
            *half = table->halves[i];
        }
    }
}

void SUG_freeValueHistory(void **obj) {
    obj[0] = (void *)freeHeapBlock(obj[0]);
    obj[1] = (void *)freeHeapBlock(obj[1]);
    freeHeapBlock(obj);
}
