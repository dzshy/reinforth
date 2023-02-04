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

#ifndef SMALL_FORTH_OPCODE_H_
#define SMALL_FORTH_OPCODE_H_

#include "data_types.h"

enum opcode {
    OP_ADD,
    OP_DOT,
    OP_CALL,
    OP_PUSH,
    OP_CREATE,
    OP_BYE,
    OP_RET,
    OP_NOP,
};

struct forth_vm;

void op_add(struct forth_vm *vm);
void op_dot(struct forth_vm *vm);
void op_call(struct forth_vm *vm);
void op_push(struct forth_vm *vm);
void op_create(struct forth_vm *vm);
void op_bye(struct forth_vm *vm);
void op_ret(struct forth_vm *vm);
void op_nop(struct forth_vm *vm);

char *get_opname(enum opcode);

opfunc get_opfunc(enum opcode op);
data get_opaddr(enum opcode op);
#endif
