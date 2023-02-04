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

#include "opcode.h"

#include "vm.h"

char *op_vec[] = {"+", ".", "call", "push", "create", "bye", "ret", "nop"};

opfunc op_funcvec[] = {
    op_add, op_dot, op_call, op_push, op_create, op_bye, op_ret, op_nop,
};

char *get_opname(enum opcode op) { return op_vec[(int)op]; }

opfunc get_opfunc(enum opcode op) { return op_funcvec[(int)op]; }

data get_opaddr(enum opcode op)
{
    opfunc f = op_funcvec[(int)op];
    return *(data *)&f;
}

void op_add(struct forth_vm *vm)
{
    data a, b;
    a = vm_pop_ds(vm);
    b = vm_pop_ds(vm);
    vm_push_ds(vm, a + b);
}

void op_dot(struct forth_vm *vm)
{
    data a = vm_pop_ds(vm);
    fprintf(vm->out, "%ld", a);
}

void op_push(struct forth_vm *vm)
{
    vm->pc++;
    data a = vm->code[vm->pc];
    vm_push_ds(vm, a);
}

void op_bye(struct forth_vm *vm)
{
    vm->finished = true;
    vm->ret = 0;
}

void op_create(struct forth_vm *vm)
{
    data a = vm_read_word(vm);
    data b = vm->codesz;
    vm_push_code(vm, get_opaddr(OP_PUSH));
    vm_push_code(vm, vm->heaptop);
    vm_push_code(vm, get_opaddr(OP_RET));
    vm->dict[a] = b;
}

void op_ret(struct forth_vm *vm) {}
void op_call(struct forth_vm *vm) {}
void op_nop(struct forth_vm *vm) {}
