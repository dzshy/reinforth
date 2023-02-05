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

#include "syntax.h"

#include <string.h>

#include "opcode.h"
#include "vm.h"

int get_syntax(char *word) {
    if (strcmp(word, ":") == 0) {
        return SYN_COLON;
    } else if (strcmp(word, ";") == 0) {
        return SYN_SEMI;
    } else if (strcmp(word, "begin") == 0) {
        return SYN_BEGIN;
    } else if (strcmp(word, "until") == 0) {
        return SYN_UNTIL;
    } else if (strcmp(word, "if") == 0) {
        return SYN_IF;
    } else if (strcmp(word, "else") == 0) {
        return SYN_ELSE;
    } else if (strcmp(word, "then") == 0) {
        return SYN_THEN;
    }
    return -1;
}


opfunc syntax_ops[] = {
    syn_colon,
    syn_semi,
    syn_begin,
    syn_until,
    syn_if,
    syn_else,
    syn_then,
    syn_nop,
};

opfunc get_syntax_op(enum syntax s)
{
    if (s > SYN_NOP) return syn_nop;
    return syntax_ops[(int)s];
}

void syn_nop(struct forthvm *vm) {}

void syn_colon(struct forthvm *vm) {
    vm_execute(vm);
    if (vm->rsp > 0) {
        vm->errmsg = "wrong place to start word definition";
        vm->finished = true;
        vm->ret = -1;
        return;
    }
    data entry = vm_read_word(vm);
    vm->dict[entry] = vm->codesz;
    vm_push_rs(vm, SYN_COLON);
    vm->ready = false;
}

void syn_semi(struct forthvm *vm) {
    enum syntax s = vm_pop_rs(vm);
    if (vm->finished) return;
    if (s != SYN_COLON) {
        vm->errmsg = "unpaired semicolon";
        vm->finished = true;
        vm->ret = -1;
        return;
    }
    vm_emit_opcode(vm, OP_RET);
    vm->pc = vm->codesz;
    vm->ready = true;
}

void syn_begin(struct forthvm *vm) {
}

void syn_until(struct forthvm *vm) {
}

void syn_if(struct forthvm *vm) {
    vm_emit_opcode(vm, OP_JZ);
    vm_push_rs(vm, vm->codesz);
    vm_emit_data(vm, -1);
    vm_push_rs(vm, SYN_IF);
}

void syn_else(struct forthvm *vm) {
    data d = vm_pop_rs(vm);
    if (d != SYN_IF || vm->finished) {
        vm->errmsg = "unpaired else";
        vm->finished = true;
        vm->ret = -1;
        return;
    }
    d = vm_pop_rs(vm);
    vm_emit_opcode(vm, OP_PUSH);
    vm_push_rs(vm, vm->codesz);
    vm_push_rs(vm, SYN_ELSE);
    vm_emit_opcode(vm, -1);
    vm_emit_opcode(vm, OP_JMP);
    vm->code[d] = vm->codesz;
}

void syn_then(struct forthvm *vm) {
    data d = vm_pop_rs(vm);
    if (vm->finished) return;
    if (d != SYN_IF && d != SYN_ELSE) {
        // vm->errmsg = "unpaired then";
        vm->finished = true;
        vm->ret = -1;
        return;
    }
    d = vm_pop_rs(vm);
    if (vm->finished) {
        return;
    }
    vm->code[d] = vm->codesz;
}

