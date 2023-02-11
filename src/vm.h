/* Copyright (c) 2023, ~dzshy <dzshy@outlook.com>
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

#ifndef REINFORTH_VM_H_
#define REINFORTH_VM_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "htable.h"
#include "opcode.h"
#include "syntax.h"
#include "types.h"

struct forthvm {
    data *ds;
    data *rs;
    void *heap;
    data *dict;
    data *code;
    HTable *wordtable;

    data pc;
    data dsp;
    data rsp;
    void *heaptop;
    data ret;

    data codesz;
    data dictsz;
    data wordreposz;

    data dscap;
    data rscap;
    data dictcap;
    data heapcap;
    data codecap;
    data linenum;

    bool ready;
    bool finished;
    FILE *in;
    FILE *out;
    char *curword;
    char *errmsg;
};

data vm_pop_ds(struct forthvm *vm);
void vm_push_ds(struct forthvm *vm, data d);
data vm_pop_rs(struct forthvm *vm);
void vm_push_rs(struct forthvm *vm, data d);
data vm_read_word(struct forthvm *vm);
void vm_emit_data(struct forthvm *vm, data d);
void vm_emit_opcode(struct forthvm *vm, enum opcode);
void vm_heap_grow(struct forthvm *vm, data size);
char vm_getc(struct forthvm *vm);
void vm_ungetc(struct forthvm *vm, char c);
void vm_regfunc(struct forthvm *vm, char *word, opfunc f);

data vm_execute(struct forthvm *vm);
void vm_init(struct forthvm *vm, FILE *fin, FILE *fout);
void vm_run(struct forthvm *vm);

#endif
