/* Copyright (c) 2022-2023, ~dzshy <dzshy@outlook.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include "htable.h"

#include <stdlib.h>
#include <string.h>

#define HTFL_NUL 0
#define HTFL_VAL 1
#define HTFL_DEL 2

static void *htable_end(HTable *ht)
{
    return ht->buf + ht->cap * (ht->elemsz + 1);
}

static void rebuild(HTable *ht)
{
    HTable newht;
    htable_init(&newht, ht->elemsz, ht->size * 6, ht->hash, ht->eq);
    void *iter = htable_begin(ht);
    while (iter != NULL) {
        htable_insert(&newht, iter);
        iter = htable_next(ht, iter);
    }
    free(ht->buf);
    *ht = newht;
}

static uint8_t getflag(void *iter) { return *(uint8_t *)(iter - 1); }

static void setflag(void *iter, uint8_t flag) { *(uint8_t *)(iter - 1) = flag; }

void htable_init(HTable *ht, int elemsz, int cap, uint32_t (*hash)(void *),
                 bool (*eq)(void *, void *))
{
    if (cap < 16)
        cap = 16;
    ht->buf = malloc(cap * (elemsz + 1));
    memset(ht->buf, 0, cap * (elemsz + 1));
    ht->size = 0;
    ht->cap = cap;
    ht->taken = 0;
    ht->begin = NULL;
    ht->elemsz = elemsz;
    ht->hash = hash;
    ht->eq = eq;
}

bool htable_insert(HTable *ht, void *elem)
{
    if (ht->taken + 1 > ht->cap / 2) {
        rebuild(ht);
    }
    ht->taken++;
    ht->size++;
    int hashcode = ht->hash(elem) % ht->cap;
    void *pos = ht->buf + hashcode * (ht->elemsz + 1) + 1;
    while (getflag(pos) != HTFL_NUL) {
        if (getflag(pos) == HTFL_VAL && ht->eq(pos, elem))
            return false;
        pos += ht->elemsz + 1;
        if (pos >= htable_end(ht)) { // arrived end, restart from beginning
            pos = ht->buf + 1;
        }
    }
    memcpy(pos, elem, ht->elemsz);
    setflag(pos, HTFL_VAL);
    if (pos < ht->begin || ht->begin == NULL) {
        ht->begin = pos;
    }
    return true;
}

void htable_del(HTable *ht, void *iter)
{
    ht->size--;
    setflag(iter, HTFL_DEL);
    if (iter == ht->begin) {
        ht->begin = htable_next(ht, iter);
    }
}

void *htable_find(HTable *ht, void *elem)
{
    int hashcode = ht->hash(elem) % ht->cap;
    void *pos = ht->buf + hashcode * (ht->elemsz + 1) + 1;
    while (getflag(pos) != HTFL_NUL) {
        if (getflag(pos) == HTFL_VAL && ht->eq(pos, elem))
            return pos;
        pos += ht->elemsz + 1;
        if (pos >= htable_end(ht)) { // arrived end, restart from beginning
            pos = ht->buf + 1;
        }
    }
    return NULL;
}

void *htable_begin(HTable *ht) { return ht->begin; }

void *htable_next(HTable *ht, void *iter)
{
    void *pos = iter;
    do {
        pos += ht->elemsz + 1;
        if (pos >= htable_end(ht)) {
            return NULL;
        }
    } while (getflag(pos) != HTFL_VAL);
    return pos;
}
