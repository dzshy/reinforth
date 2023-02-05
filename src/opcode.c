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

char *op_vec[] = {"+", "-", "dup", "over", "swap", "drop", ".", "call", "push", "create", "bye", "ret", "jmp", "jz", "nop"};

opfunc op_funcvec[] = {
    op_add, op_minus, op_dup, op_over, op_swap, op_drop, op_dot, op_call, op_push, op_create, op_bye, op_ret, op_jmp, op_jz, op_nop,
};

char *get_opname(enum opcode op) { return op_vec[(int)op]; }

opfunc get_opfunc(enum opcode op) { return op_funcvec[(int)op]; }

data get_opaddr(enum opcode op)
{
    opfunc f = op_funcvec[(int)op];
    return *(data *)&f;
}

void op_add(struct forthvm *vm)
{
    data a, b;
    a = vm_pop_ds(vm);
    if (vm->finished) return;
    b = vm_pop_ds(vm);
    if (vm->finished) return;
    vm_push_ds(vm, a + b);
}

void op_minus(struct forthvm *vm) {
    data a, b;
    b = vm_pop_ds(vm);
    if (vm->finished) return;
    a = vm_pop_ds(vm);
    if (vm->finished) return;
    vm_push_ds(vm, a - b);
}

void op_dup(struct forthvm *vm) {
    data a;
    a = vm_pop_ds(vm);
    if (vm->finished) return;
    vm_push_ds(vm, a);
    vm_push_ds(vm, a);
}

void op_over(struct forthvm *vm) {
    data a, b;
    a = vm_pop_ds(vm);
    if (vm->finished) return;
    b = vm_pop_ds(vm);
    if (vm->finished) return;
    vm_push_ds(vm, b);
    vm_push_ds(vm, a);
    vm_push_ds(vm, b);
}

void op_swap(struct forthvm *vm) {
    data a, b;
    a = vm_pop_ds(vm);
    if (vm->finished) return;
    b = vm_pop_ds(vm);
    if (vm->finished) return;
    vm_push_ds(vm, a);
    vm_push_ds(vm, b);
}

void op_drop(struct forthvm *vm) {
    vm_pop_ds(vm);
}

void op_dot(struct forthvm *vm)
{
    data a = vm_pop_ds(vm);
    if (vm->finished) return;
    fprintf(vm->out, "%ld ", a);
}

void op_push(struct forthvm *vm)
{
    vm->pc++;
    data a = vm->code[vm->pc];
    vm_push_ds(vm, a);
}

void op_bye(struct forthvm *vm)
{
    vm->finished = true;
    vm->ret = 0;
}

void op_create(struct forthvm *vm)
{
    vm_emit_opcode(vm, OP_PUSH);
    vm_emit_data(vm, 0);
    data *jmp_ptr = &vm->code[vm->codesz - 1];
    vm_emit_opcode(vm, OP_JMP);
    data a = vm_read_word(vm);
    data b = vm->codesz;
    vm->dict[a] = b;
    vm_emit_opcode(vm, OP_PUSH);
    vm_emit_data(vm, vm->heaptop);
    vm_emit_opcode(vm, OP_RET);
    *jmp_ptr = vm->codesz;
}

void op_jmp(struct forthvm *vm) {
    data addr = vm_pop_ds(vm);
    vm->pc = addr - 1;
}

void op_jz(struct forthvm *vm) {
    vm->pc++;
    data addr = vm->code[vm->pc];
    data d = vm_pop_ds(vm);
    if (vm->finished) return;
    if (d == 0) vm->pc = addr - 1;
}

void op_ret(struct forthvm *vm)
{
    data addr = vm_pop_rs(vm);
    vm->pc = addr;
}

void op_call(struct forthvm *vm)
{
    vm_push_rs(vm, vm->pc);
    data entry = vm_pop_ds(vm);
    if (vm->finished) return;
    data addr = vm->dict[entry];
    if (addr < 0) {
        vm->errmsg = "op_call invlid address";
        vm->finished = true;
        vm->ret = -1;
        return;
    }
    vm->pc = addr - 1;
}

void op_nop(struct forthvm *vm) {}
