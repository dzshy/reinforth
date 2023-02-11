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

#include "types.h"

enum opcode {
    OP_ADD,
    OP_MINUS,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_DIVMOD,
    OP_MIN,
    OP_MAX,
    OP_NEGATE,
    OP_EQ,
    OP_NEQ,
    OP_GT,
    OP_LT,
    OP_GE,
    OP_LE,
    OP_AND,
    OP_OR,
    OP_NOT,
    OP_BITAND,
    OP_BITOR,
    OP_INVERT,
    OP_XOR,
    OP_DUP,
    OP_OVER,
    OP_SWAP,
    OP_DROP,
    OP_DOT,
    OP_CALL,
    OP_PUSH,
    OP_CREATE,
    OP_BYE,
    OP_EXIT,
    OP_JMP,
    OP_JZ,
    OP_CELLS,
    OP_CHARS,
    OP_ALLOT,
    OP_ALLOCATE,
    OP_RESIZE,
    OP_FREE,
    OP_BANG,
    OP_AT,
    OP_COMMA,
    OP_HERE,
    OP_CR,
    OP_PRINT,
    OP_EMIT,
    OP_ASSERT,
    OP_ROT,
    OP_DUMP,
    OP_RDUMP,
    OP_DEPTH,
    OP_PICK,
    OP_RPICK,
    OP_D2R,
    OP_R2D,
    OP_RAT,
    OP_EXECUTE,
    OP_QUOTE,
    OP_CFUNC,
    OP_NOP,
};

struct forthvm;

void op_execute(struct forthvm *vm);
void op_quote(struct forthvm *vm);
void op_cfunc(struct forthvm *vm);
void op_pick(struct forthvm *vm);
void op_rpick(struct forthvm *vm);
void op_r2d(struct forthvm *vm);
void op_d2r(struct forthvm *vm);
void op_rat(struct forthvm *vm);
void op_dump(struct forthvm *vm);
void op_rdump(struct forthvm *vm);
void op_depth(struct forthvm *vm);
void op_emit(struct forthvm *vm);
void op_assert(struct forthvm *vm);
void op_rot(struct forthvm *vm);
void op_cr(struct forthvm *vm);
void op_comma(struct forthvm *vm);
void op_here(struct forthvm *vm);
void op_add(struct forthvm *vm);
void op_minus(struct forthvm *vm);
void op_mul(struct forthvm *vm);
void op_div(struct forthvm *vm);
void op_mod(struct forthvm *vm);
void op_divmod(struct forthvm *vm);
void op_min(struct forthvm *vm);
void op_max(struct forthvm *vm);
void op_negate(struct forthvm *vm);
void op_eq(struct forthvm *vm);
void op_neq(struct forthvm *vm);
void op_gt(struct forthvm *vm);
void op_lt(struct forthvm *vm);
void op_ge(struct forthvm *vm);
void op_le(struct forthvm *vm);
void op_and(struct forthvm *vm);
void op_or(struct forthvm *vm);
void op_not(struct forthvm *vm);
void op_bitand(struct forthvm *vm);
void op_bitor(struct forthvm *vm);
void op_invert(struct forthvm *vm);
void op_xor(struct forthvm *vm);
void op_dup(struct forthvm *vm);
void op_over(struct forthvm *vm);
void op_swap(struct forthvm *vm);
void op_drop(struct forthvm *vm);
void op_dot(struct forthvm *vm);
void op_call(struct forthvm *vm);
void op_push(struct forthvm *vm);
void op_create(struct forthvm *vm);
void op_bye(struct forthvm *vm);
void op_exit(struct forthvm *vm);
void op_jmp(struct forthvm *vm);
void op_jz(struct forthvm *vm);
void op_cells(struct forthvm *vm);
void op_chars(struct forthvm *vm);
void op_allot(struct forthvm *vm);
void op_allocate(struct forthvm *vm);
void op_resize(struct forthvm *vm);
void op_free(struct forthvm *vm);
void op_bang(struct forthvm *vm);
void op_at(struct forthvm *vm);
void op_print(struct forthvm *vm);
void op_nop(struct forthvm *vm);

char *get_opname(enum opcode);

opfunc get_opfunc(enum opcode op);
data get_opaddr(enum opcode op);

#endif
