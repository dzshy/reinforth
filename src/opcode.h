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

#ifndef REINFORTH_OPCODE_H_
#define REINFORTH_OPCODE_H_

#include "data_types.h"

enum opcode {
    OP_ADD,
    OP_MINUS,
    OP_EQ,
    OP_GT,
    OP_LT,
    OP_GE,
    OP_LE,
    OP_DUP,
    OP_OVER,
    OP_SWAP,
    OP_DROP,
    OP_DOT,
    OP_CALL,
    OP_PUSH,
    OP_CREATE,
    OP_BYE,
    OP_RET,
    OP_JMP,
    OP_JZ,
    OP_ALLOT,
    OP_BANG,
    OP_AT,
    OP_NOP,
};

struct forthvm;

void op_add(struct forthvm *vm);
void op_minus(struct forthvm *vm);
void op_eq(struct forthvm *vm);
void op_gt(struct forthvm *vm);
void op_lt(struct forthvm *vm);
void op_ge(struct forthvm *vm);
void op_le(struct forthvm *vm);
void op_dup(struct forthvm *vm);
void op_over(struct forthvm *vm);
void op_swap(struct forthvm *vm);
void op_drop(struct forthvm *vm);
void op_dot(struct forthvm *vm);
void op_call(struct forthvm *vm);
void op_push(struct forthvm *vm);
void op_create(struct forthvm *vm);
void op_bye(struct forthvm *vm);
void op_ret(struct forthvm *vm);
void op_jmp(struct forthvm *vm);
void op_jz(struct forthvm *vm);
void op_allot(struct forthvm *vm);
void op_bang(struct forthvm *vm);
void op_at(struct forthvm *vm);
void op_nop(struct forthvm *vm);

char *get_opname(enum opcode);

opfunc get_opfunc(enum opcode op);
data get_opaddr(enum opcode op);

#endif
