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

#define CHECKERR                                                               \
    if (unlikely(vm->finished))                                                \
        return;

#define CHECKDS(len)                                                           \
    if (unlikely(vm->dsp < len)) {                                             \
        vm->ret = -1;                                                          \
        vm->finished = true;                                                   \
        vm->errmsg = "no enough element on data stack";                        \
        return;                                                                \
    }

char *op_vec[] = {"+",    "-",    ">",    "<",  ">=",   "<=",   "dup",
                  "over", "swap", "drop", ".",  "call", "push", "create",
                  "bye",  "ret",  "jmp",  "jz", "nop"};

opfunc op_funcvec[] = {
    op_add,  op_minus, op_gt,   op_lt,  op_ge,   op_le,   op_dup,
    op_over, op_swap,  op_drop, op_dot, op_call, op_push, op_create,
    op_bye,  op_ret,   op_jmp,  op_jz,  op_nop,
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
    CHECKERR;
    b = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, a + b);
}

void op_minus(struct forthvm *vm)
{
    data a, b;
    b = vm_pop_ds(vm);
    CHECKERR;
    a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, a - b);
}

void op_gt(struct forthvm *vm)
{
    data a, b;
    b = vm_pop_ds(vm);
    CHECKERR;
    a = vm_pop_ds(vm);
    CHECKERR;
    data r;
    if (a > b) {
        r = -1;
    } else {
        r = 0;
    }
    vm_push_ds(vm, r);
}

void op_lt(struct forthvm *vm)
{
    data a, b;
    b = vm_pop_ds(vm);
    CHECKERR;
    a = vm_pop_ds(vm);
    CHECKERR;
    data r;
    if (a < b) {
        r = -1;
    } else {
        r = 0;
    }
    vm_push_ds(vm, r);
}

void op_ge(struct forthvm *vm)
{
    data a, b;
    b = vm_pop_ds(vm);
    CHECKERR;
    a = vm_pop_ds(vm);
    CHECKERR;
    data r;
    if (a >= b) {
        r = -1;
    } else {
        r = 0;
    }
    vm_push_ds(vm, r);
}

void op_le(struct forthvm *vm)
{
    data a, b;
    b = vm_pop_ds(vm);
    CHECKERR;
    a = vm_pop_ds(vm);
    CHECKERR;
    data r;
    if (a <= b) {
        r = -1;
    } else {
        r = 0;
    }
    vm_push_ds(vm, r);
}

void op_dup(struct forthvm *vm)
{
    CHECKDS(1);
    data a;
    a = vm->ds[vm->dsp];
    vm_push_ds(vm, a);
}

void op_over(struct forthvm *vm)
{
    CHECKDS(2);
    data a = vm->ds[vm->dsp - 1];
    vm_push_ds(vm, a);
}

void op_swap(struct forthvm *vm)
{
    CHECKDS(2);
    data t;
    t = vm->ds[vm->dsp];
    vm->ds[vm->dsp] = vm->ds[vm->dsp - 1];
    vm->ds[vm->dsp - 1] = t;
}

void op_drop(struct forthvm *vm)
{
    CHECKDS(1);
    vm->dsp--;
}

void op_dot(struct forthvm *vm)
{
    data a = vm_pop_ds(vm);
    CHECKERR;
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
    vm_emit_opcode(vm, OP_JMP);
    vm_emit_data(vm, 0);
    data *jmp_ptr = &vm->code[vm->codesz - 1];
    data a = vm_read_word(vm);
    data b = vm->codesz;
    vm->dict[a] = b;
    vm_emit_opcode(vm, OP_PUSH);
    vm_emit_data(vm, vm->heaptop);
    vm_emit_opcode(vm, OP_RET);
    *jmp_ptr = vm->codesz;
}

void op_jmp(struct forthvm *vm)
{
    vm->pc++;
    data addr = vm->code[vm->pc];
    if (addr < 0) {
        vm->finished = true;
        vm->ret = -1;
        vm->errmsg = "failed to jmp, invalid address";
    }
    vm->pc = addr - 1;
}

void op_jz(struct forthvm *vm)
{
    vm->pc++;
    data addr = vm->code[vm->pc];
    if (addr < 0) {
        vm->finished = true;
        vm->ret = -1;
        vm->errmsg = "failed to jz, invalid address";
    }
    data d = vm_pop_ds(vm);
    CHECKERR;
    if (d == 0)
        vm->pc = addr - 1;
}

void op_ret(struct forthvm *vm)
{
    data addr = vm_pop_rs(vm);
    vm->pc = addr;
}

void op_call(struct forthvm *vm)
{
    vm->pc++;
    data entry = vm->code[vm->pc];
    data addr = vm->dict[entry];
    if (addr < 0) {
        vm->finished = true;
        vm->ret = -1;
        vm->errmsg = "undefined word";
    }
    vm_push_rs(vm, vm->pc);
    vm->pc = addr - 1;
}

void op_nop(struct forthvm *vm) {}
