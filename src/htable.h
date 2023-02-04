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

#ifndef HTABLE_H_
#define HTABLE_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    void *buf;
    int size;
    int cap;
    int taken;
    void *begin;
    int elemsz;
    uint32_t (*hash)(void *);
    bool (*eq)(void *, void *);
} HTable;

void htable_init(HTable *ht, int elemsz, int cap, uint32_t (*hash)(void *),
                 bool (*eq)(void *, void *));
bool htable_insert(HTable *ht, void *elem);
void htable_del(HTable *ht, void *iter);

// return a iterator
void *htable_find(HTable *ht, void *elem);
void *htable_begin(HTable *ht);
void *htable_next(HTable *ht, void *iter);

#endif
