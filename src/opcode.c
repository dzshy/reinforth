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

char *op_vec[] = {
    "+",      "-",    "*",      "/",     "mod",    "/mod",     "min",    "max",
    "negate", "=",    "<>",     ">",     "<",      ">=",       "<=",     "and",
    "or",     "not",  "bitand", "bitor", "invert", "xor",      "dup",    "over",
    "swap",   "drop", ".",      "call",  "push",   "create",   "bye",    "ret",
    "jmp",    "jz",   "cells",  "chars", "allot",  "allocate", "resize", "free",
    "!",      "@",    "nop"};

opfunc op_funcvec[] = {
    op_add,    op_minus,    op_mul,    op_div,  op_mod,   op_divmod,
    op_min,    op_max,      op_negate, op_eq,   op_neq,   op_gt,
    op_lt,     op_ge,       op_le,     op_and,  op_or,    op_not,
    op_bitand, op_bitor,    op_invert, op_xor,  op_dup,   op_over,
    op_swap,   op_drop,     op_dot,    op_call, op_push,  op_create,
    op_bye,    op_ret,      op_jmp,    op_jz,   op_cells, op_chars,
    op_allot,  op_allocate, op_resize, op_free, op_bang,  op_at,
    op_nop,
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

void op_mul(struct forthvm *vm)
{
    data a, b;
    b = vm_pop_ds(vm);
    CHECKERR;
    a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, a * b);
}

void op_div(struct forthvm *vm)
{
    data a, b;
    b = vm_pop_ds(vm);
    CHECKERR;
    a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, a / b);
}

void op_mod(struct forthvm *vm)
{
    data a, b;
    b = vm_pop_ds(vm);
    CHECKERR;
    a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, a % b);
}

void op_divmod(struct forthvm *vm)
{
    data a, b;
    b = vm_pop_ds(vm);
    CHECKERR;
    a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, a % b);
    vm_push_ds(vm, a / b);
}

void op_min(struct forthvm *vm)
{
    data a, b;
    b = vm_pop_ds(vm);
    CHECKERR;
    a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, a < b ? a : b);
}

void op_max(struct forthvm *vm)
{
    data a, b;
    b = vm_pop_ds(vm);
    CHECKERR;
    a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, a > b ? a : b);
}

void op_negate(struct forthvm *vm)
{
    data a;
    a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, -a);
}

void op_eq(struct forthvm *vm)
{
    data a, b;
    a = vm_pop_ds(vm);
    CHECKERR;
    b = vm_pop_ds(vm);
    CHECKERR;
    if (a == b)
        vm_push_ds(vm, -1);
    else
        vm_push_ds(vm, 0);
}

void op_neq(struct forthvm *vm)
{
    data a, b;
    a = vm_pop_ds(vm);
    CHECKERR;
    b = vm_pop_ds(vm);
    CHECKERR;
    if (a == b)
        vm_push_ds(vm, 0);
    else
        vm_push_ds(vm, -1);
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

void op_and(struct forthvm *vm)
{
    data a, b;
    b = vm_pop_ds(vm);
    CHECKERR;
    a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, a && b ? -1 : 0);
}

void op_or(struct forthvm *vm)
{
    data a, b;
    b = vm_pop_ds(vm);
    CHECKERR;
    a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, a || b ? -1 : 0);
}

void op_not(struct forthvm *vm)
{
    data a;
    a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, a ? 0 : -1);
}

void op_bitand(struct forthvm *vm)
{
    data a, b;
    b = vm_pop_ds(vm);
    CHECKERR;
    a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, a & b);
}

void op_bitor(struct forthvm *vm)
{
    data a, b;
    b = vm_pop_ds(vm);
    CHECKERR;
    a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, a | b);
}

void op_invert(struct forthvm *vm)
{
    data a;
    a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, ~a);
}

void op_xor(struct forthvm *vm)
{
    data a, b;
    b = vm_pop_ds(vm);
    CHECKERR;
    a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, a ^ b);
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
    vm_emit_data(vm, (data)vm->heaptop);
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

void op_cells(struct forthvm *vm)
{
    data a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, a * sizeof(data));
}

void op_chars(struct forthvm *vm)
{
    data a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, a * sizeof(char));
}

void op_allot(struct forthvm *vm)
{
    data size = vm_pop_ds(vm);
    CHECKERR;
    vm_heap_grow(vm, size);
    CHECKERR;
}

void op_allocate(struct forthvm *vm)
{
    data size = vm_pop_ds(vm);
    CHECKERR;
    void *buf = malloc(size);
    vm_push_ds(vm, (data)buf);
    CHECKERR;
}

void op_resize(struct forthvm *vm)
{
    CHECKDS(2);
    data size = vm_pop_ds(vm);
    data addr = vm_pop_ds(vm);
    void *buf = (void *)addr;
    buf = realloc(buf, size);
    vm_push_ds(vm, (data)buf);
}

void op_free(struct forthvm *vm)
{
    data addr = vm_pop_ds(vm);
    CHECKERR;
    void *buf = (void *)addr;
    free(buf);
}

void op_bang(struct forthvm *vm)
{
    data addr = vm_pop_ds(vm);
    CHECKERR;
    data x = vm_pop_ds(vm);
    CHECKERR;
    *(data *)addr = x;
}

void op_at(struct forthvm *vm)
{
    data addr = vm_pop_ds(vm);
    CHECKERR;
    vm_push_ds(vm, *(data *)addr);
}

void op_nop(struct forthvm *vm) {}
