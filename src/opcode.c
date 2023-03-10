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

#include <assert.h>
#include <string.h>

#include "vm.h"

#define CHECKERR                                                               \
    if (vm->finished)                                                          \
        return;

#define CHECKDS(len)                                                           \
    if (vm->dsp < len) {                                                       \
        vm->ret = -1;                                                          \
        vm->finished = true;                                                   \
        vm->errmsg = "no enough element on data stack";                        \
        return;                                                                \
    }

#define CHECKRS(len)                                                           \
    if (vm->rsp < len) {                                                       \
        vm->ret = -1;                                                          \
        vm->finished = true;                                                   \
        vm->errmsg = "no enough element on return stack";                      \
        return;                                                                \
    }

char *op_vec[OP_NOP + 1] = {
    [OP_DUMP] = "dump",
    [OP_RDUMP] = "rdump",
    [OP_DEPTH] = "depth",
    [OP_EMIT] = "emit",
    [OP_ASSERT] = "assert",
    [OP_ROT] = "rot",
    [OP_PRINT] = "print",
    [OP_COMMA] = ",",
    [OP_HERE] = "here",
    [OP_CR] = "cr",
    [OP_ADD] = "+",
    [OP_MINUS] = "-",
    [OP_MUL] = "*",
    [OP_DIV] = "/",
    [OP_MOD] = "mod",
    [OP_DIVMOD] = "/mod",
    [OP_MIN] = "min",
    [OP_MAX] = "max",
    [OP_NEGATE] = "negate",
    [OP_EQ] = "=",
    [OP_NEQ] = "<>",
    [OP_GT] = ">",
    [OP_LT] = "<",
    [OP_GE] = ">=",
    [OP_LE] = "<=",
    [OP_AND] = "and",
    [OP_OR] = "or",
    [OP_OR] = "or",
    [OP_NOT] = "not",
    [OP_BITAND] = "bitand",
    [OP_BITOR] = "bitor",
    [OP_INVERT] = "invert",
    [OP_XOR] = "xor",
    [OP_DUP] = "dup",
    [OP_OVER] = "over",
    [OP_SWAP] = "swap",
    [OP_DROP] = "drop",
    [OP_DOT] = ".",
    [OP_CALL] = "call\t",
    [OP_PUSH] = "push\t",
    [OP_CREATE] = "create",
    [OP_BYE] = "bye",
    [OP_EXIT] = "exit",
    [OP_JMP] = "jmp\t",
    [OP_JZ] = "jz\t",
    [OP_CELLS] = "cells",
    [OP_CHARS] = "chars",
    [OP_ALLOT] = "allot",
    [OP_ALLOCATE] = "allocate",
    [OP_RESIZE] = "resize",
    [OP_FREE] = "free",
    [OP_BANG] = "!",
    [OP_AT] = "@",
    [OP_NOP] = "nop\t",
    [OP_PICK] = "pick",
    [OP_RPICK] = "rpick",
    [OP_R2D] = "r>",
    [OP_D2R] = ">r",
    [OP_RAT] = "r@",
    [OP_EXECUTE] = "execute",
    [OP_QUOTE] = "'",
    [OP_CFUNC] = "cfunc\t",
    [OP_DO] = "do\t",
    [OP_LOOP] = "loop\t",
    [OP_PLUSLOOP] = "+loop\t",
    [OP_CFUNC] = "cfunc\t",
    [OP_I] = "i",
    [OP_II] = "i'",
    [OP_J] = "j",
    [OP_HEAPSIZE] = "heap-size",
};

opfunc op_funcvec[OP_NOP + 1] = {
    [OP_I] = op_i,
    [OP_II] = op_ii,
    [OP_J] = op_j,
    [OP_DO] = op_do,
    [OP_LOOP] = op_loop,
    [OP_PLUSLOOP] = op_plusloop,
    [OP_EXECUTE] = op_execute,
    [OP_QUOTE] = op_quote,
    [OP_CFUNC] = op_cfunc,
    [OP_PICK] = op_pick,
    [OP_RPICK] = op_rpick,
    [OP_R2D] = op_r2d,
    [OP_D2R] = op_d2r,
    [OP_RAT] = op_rat,
    [OP_DUMP] = op_dump,
    [OP_RDUMP] = op_rdump,
    [OP_DEPTH] = op_depth,
    [OP_ASSERT] = op_assert,
    [OP_EMIT] = op_emit,
    [OP_ROT] = op_rot,
    [OP_PRINT] = op_print,
    [OP_COMMA] = op_comma,
    [OP_HERE] = op_here,
    [OP_CR] = op_cr,
    [OP_ADD] = op_add,
    [OP_MINUS] = op_minus,
    [OP_MUL] = op_mul,
    [OP_DIV] = op_div,
    [OP_MOD] = op_mod,
    [OP_DIVMOD] = op_divmod,
    [OP_MIN] = op_min,
    [OP_MAX] = op_max,
    [OP_NEGATE] = op_negate,
    [OP_EQ] = op_eq,
    [OP_NEQ] = op_neq,
    [OP_GT] = op_gt,
    [OP_LT] = op_lt,
    [OP_GE] = op_ge,
    [OP_LE] = op_le,
    [OP_AND] = op_and,
    [OP_OR] = op_or,
    [OP_NOT] = op_not,
    [OP_BITAND] = op_bitand,
    [OP_BITOR] = op_bitor,
    [OP_INVERT] = op_invert,
    [OP_XOR] = op_xor,
    [OP_DUP] = op_dup,
    [OP_OVER] = op_over,
    [OP_SWAP] = op_swap,
    [OP_DROP] = op_drop,
    [OP_DOT] = op_dot,
    [OP_CALL] = op_call,
    [OP_PUSH] = op_push,
    [OP_CREATE] = op_create,
    [OP_BYE] = op_bye,
    [OP_EXIT] = op_exit,
    [OP_JMP] = op_jmp,
    [OP_JZ] = op_jz,
    [OP_CELLS] = op_cells,
    [OP_CHARS] = op_chars,
    [OP_ALLOT] = op_allot,
    [OP_ALLOCATE] = op_allocate,
    [OP_RESIZE] = op_resize,
    [OP_FREE] = op_free,
    [OP_BANG] = op_bang,
    [OP_AT] = op_at,
    [OP_NOP] = op_nop,
    [OP_HEAPSIZE] = op_heapsize,
};

char *get_opname(enum opcode op) { return op_vec[(int)op]; }

opfunc get_opfunc(enum opcode op) { return op_funcvec[(int)op]; }

data get_opaddr(enum opcode op)
{
    opfunc f = op_funcvec[(int)op];
    return *(data *)&f;
}

void op_heapsize(struct forthvm *vm)
{
    data sz = vm_pop_ds(vm);
    CHECKERR;
    vm_heapsz(vm, sz);
}

void op_i(struct forthvm *vm)
{
    CHECKRS(1);
    vm_push_ds(vm, vm->rs[vm->rsp]);
}

void op_ii(struct forthvm *vm)
{
    CHECKRS(2);
    vm_push_ds(vm, vm->rs[vm->rsp - 1]);
}

void op_j(struct forthvm *vm)
{
    CHECKRS(3);
    vm_push_ds(vm, vm->rs[vm->rsp - 2]);
}

void op_do(struct forthvm *vm)
{
    CHECKRS(2);
    data i = vm->rs[vm->rsp];
    data limit = vm->rs[vm->rsp - 1];
    vm->pc++;
    data end = vm->code[vm->pc];
    if (i < limit) {
        return;
    }
    vm->pc = end - 1;
}

void op_loop(struct forthvm *vm) { vm->rs[vm->rsp]++; }

void op_plusloop(struct forthvm *vm)
{
    data inc = vm_pop_ds(vm);
    CHECKERR;
    CHECKRS(1);
    vm->rs[vm->rsp] += inc;
}

void op_pick(struct forthvm *vm)
{
    data a = vm_pop_ds(vm);
    CHECKERR;
    CHECKDS(a);
    vm_push_ds(vm, vm->ds[vm->dsp - a]);
}

void op_rpick(struct forthvm *vm)
{
    data a = vm_pop_ds(vm);
    CHECKERR;
    CHECKRS(a);
    vm_push_ds(vm, vm->rs[vm->rsp - a]);
}

void op_d2r(struct forthvm *vm)
{
    data a = vm_pop_ds(vm);
    CHECKERR;
    vm_push_rs(vm, a);
}

void op_r2d(struct forthvm *vm)
{
    data a = vm_pop_rs(vm);
    CHECKERR;
    vm_push_ds(vm, a);
}

void op_rat(struct forthvm *vm)
{
    CHECKRS(1);
    vm_push_ds(vm, vm->rs[vm->rsp]);
}

void op_dump(struct forthvm *vm)
{
    data depth = vm->dsp;
    fprintf(vm->out, "<%lld> ", depth);
    for (int i = 1; i <= depth; i++) {
        fprintf(vm->out, "%ld ", vm->ds[i]);
    }
    fprintf(vm->out, "<top>");
}

void op_rdump(struct forthvm *vm)
{
    data depth = vm->rsp;
    fprintf(vm->out, "[%lld] ", depth);
    for (int i = 1; i <= depth; i++) {
        fprintf(vm->out, "%ld ", vm->rs[i]);
    }
    fprintf(vm->out, "[top]");
}

void op_depth(struct forthvm *vm)
{
    data depth = vm->dsp;
    vm_push_ds(vm, depth);
}

void op_emit(struct forthvm *vm)
{
    char c = vm_pop_ds(vm);
    CHECKERR;
    fprintf(vm->out, "%c", c);
}

void op_assert(struct forthvm *vm)
{
    data a = vm_pop_ds(vm);
    CHECKERR;
    if (!a) {
        vm->finished = true;
        vm->ret = -2;
    }
}

void op_rot(struct forthvm *vm)
{
    CHECKDS(3);
    data t = vm->ds[vm->dsp - 2];
    vm->ds[vm->dsp - 2] = vm->ds[vm->dsp - 1];
    vm->ds[vm->dsp - 1] = vm->ds[vm->dsp];
    vm->ds[vm->dsp] = t;
}

void op_comma(struct forthvm *vm)
{
    vm_heap_grow(vm, sizeof(data));
    CHECKERR;
    data *p = vm->heaptop - sizeof(data);
    data a = vm_pop_ds(vm);
    CHECKERR;
    *p = a;
}

void op_print(struct forthvm *vm)
{
    char *s = (char *)vm_pop_ds(vm);
    CHECKERR;
    fprintf(vm->out, "%s", s);
}

void op_here(struct forthvm *vm)
{
    data a = (data)vm->heaptop;
    vm_push_ds(vm, a);
}

void op_cr(struct forthvm *vm) { fprintf(vm->out, "\n"); }

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

void op_quote(struct forthvm *vm)
{
    data a = vm_read_word(vm);
    vm_push_ds(vm, a);
}

void op_create(struct forthvm *vm)
{
    vm_emit_opcode(vm, OP_JMP);
    vm_emit_data(vm, 0);
    data addr_ptr = vm->codesz - 1;
    data a = vm_read_word(vm);
    data b = vm->codesz;
    vm->dict[a] = b;
    vm_emit_opcode(vm, OP_PUSH);
    vm_emit_data(vm, (data)vm->heaptop);
    vm_emit_opcode(vm, OP_EXIT);
    vm->code[addr_ptr] = vm->codesz;
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

void op_exit(struct forthvm *vm)
{
    data addr = vm_pop_rs(vm);
    vm->pc = addr;
}

void op_execute(struct forthvm *vm)
{
    data entry = vm_pop_ds(vm);
    CHECKERR;
    if (entry <= OP_NOP) {
        opfunc f = get_opfunc(entry);
        (*f)(vm);
        return;
    }
    data addr = vm->dict[entry];
    if (addr < 0) {
        vm->finished = true;
        vm->ret = -1;
        vm->errmsg = "undefined word";
    }
    vm_push_rs(vm, vm->pc);
    vm->pc = addr - 1;
}

void op_cfunc(struct forthvm *vm)
{
    vm->pc++;
    data addr = vm->code[vm->pc];
    opfunc f = *(opfunc *)&addr;
    (*f)(vm);
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
