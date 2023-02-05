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

#include "vm.h"

#include <string.h>

#include "crc32.h"
#include "token.h"

struct word_entry {
    char *word;
    data entry;
};

uint32_t word_entry_hash(void *p)
{
    struct word_entry *kv = p;
    int size = strlen(kv->word);
    return crc32(0, kv->word, size);
}

bool word_entry_eq(void *a_, void *b_)
{
    struct word_entry *a = a_;
    struct word_entry *b = b_;
    if (strcmp(a->word, b->word) == 0) {
        return true;
    }
    return false;
}

static data create_word(struct forthvm *vm, char *word)
{
    char *dup_word = strdup(word);
    vm->dict[vm->dictsz] = -1;
    struct word_entry we = (struct word_entry){dup_word, vm->dictsz};
    htable_insert(vm->wordtable, &we);
    vm->dictsz++;
    return vm->dictsz - 1;
}

static data find_word(struct forthvm *vm, char *word)
{
    struct word_entry we;
    we.word = word;
    struct word_entry *iter = htable_find(vm->wordtable, &we);
    if (iter != NULL)
        return iter->entry;
    return create_word(vm, word);
}

data vm_pop_ds(struct forthvm *vm)
{
    if (unlikely(vm->dsp <= 0)) {
        vm->errmsg = "pop from data stack failed";
        vm->finished = true;
        vm->ret = -1;
        return -1;
    }
    data r = vm->ds[vm->dsp];
    vm->dsp--;
    return r;
}

void vm_push_ds(struct forthvm *vm, data d)
{
    vm->dsp++;
    vm->ds[vm->dsp] = d;
}

data vm_pop_rs(struct forthvm *vm)
{
    if (unlikely(vm->rsp <= 0)) {
        vm->errmsg = "pop from return stack failed";
        vm->finished = true;
        vm->ret = -1;
        return -1;
    }
    data r = vm->rs[vm->rsp];
    vm->rsp--;
    return r;
}

void vm_push_rs(struct forthvm *vm, data d)
{
    vm->rsp++;
    vm->rs[vm->rsp] = d;
}

void vm_emit_data(struct forthvm *vm, data d)
{
    vm->code[vm->codesz] = d;
    vm->codesz++;
}

void vm_emit_opcode(struct forthvm *vm, enum opcode op)
{
    data d = get_opaddr(op);
    vm_emit_data(vm, d);
}

void vm_init(struct forthvm *vm, FILE *fin, FILE *fout)
{
    *vm = (struct forthvm){0};
    vm->ds = malloc(1000 * sizeof(data));
    vm->rs = malloc(1000 * sizeof(data));
    vm->heap = malloc(1000 * sizeof(data));
    vm->dict = malloc(1000 * sizeof(data));
    vm->code = malloc(1000 * sizeof(data));
    vm->curword = malloc(1024);
    vm->wordtable = malloc(sizeof(HTable));
    htable_init(vm->wordtable, sizeof(struct word_entry), -1, word_entry_hash,
                word_entry_eq);
    vm->ready = true;
    vm->errmsg = "";

    for (data i = 0; i < (data)OP_NOP; i++) {
        find_word(vm, get_opname((enum opcode)i));
    }

    vm->in = fin;
    vm->out = fout;
}

data vm_execute(struct forthvm *vm)
{
    if (!vm->ready)
        return 0;
    data a, b;
    opfunc func_ptr;
    data ret = 0;
    enum opcode op;

    while (!vm->finished) {
        if (unlikely(vm->pc >= vm->codesz)) {
            break;
        }
        data op_addr = vm->code[vm->pc];
        opfunc opf = *(opfunc *)&op_addr;
        (*opf)(vm);
        vm->pc++;
    }
    return ret;
}

static int compile(struct forthvm *vm)
{
    struct token tok;
    data entry;
    while (!vm->finished) {
        tok = get_token(vm->in, vm->curword);
        switch (tok.type) {
        case TOK_NUM:
            vm_emit_opcode(vm, OP_PUSH);
            vm_emit_data(vm, tok.dat);
            break;
        case TOK_WORD:
            entry = find_word(vm, vm->curword);
            if (entry < (data)OP_NOP) {
                vm_emit_opcode(vm, entry);
            } else {
                vm_emit_opcode(vm, OP_CALL);
                vm_emit_data(vm, entry);
            }
            if (vm->ready)
                return 1;
            break;
        case TOK_SYNTAX:
            opfunc syntax = get_syntax_op(tok.dat);
            (*syntax)(vm);
            break;
        case TOK_EOF:
            vm_execute(vm);
            vm->finished = true;
            break;
        default:
            break;
        }
    }
    return 0;
}

data vm_read_word(struct forthvm *vm)
{
    struct token tok;
    tok = get_token(vm->in, vm->curword);
    if (tok.type != TOK_WORD) {
        vm->finished = true;
        vm->ret = -1;
        vm->errmsg = "next input token is expeted to be a word";
        return -1;
    }
    return find_word(vm, vm->curword);
}

void vm_run(struct forthvm *vm)
{
    data ret;
    while (!vm->finished) {
        while (!vm->finished && !compile(vm))
            ;
        vm_execute(vm);
    }
}
