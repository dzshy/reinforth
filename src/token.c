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

#include "token.h"

#include <ctype.h>
#include <stdlib.h>

#include "str.h"
#include "syntax.h"
#include "vm.h"

char peek_char(struct forthvm *vm)
{
    int c = vm_getc(vm);
    vm_ungetc(vm, c);
    return c;
}

void skipspace(struct forthvm *vm)
{
    char c = peek_char(vm);
    while (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
        vm_getc(vm);
        c = peek_char(vm);
    }
}

void skipcomment(struct forthvm *vm)
{
    char c = peek_char(vm);
    while (c != ')') {
        vm_getc(vm);
        c = peek_char(vm);
    }
    vm_getc(vm);
}

void parse_word(struct forthvm *vm)
{
    int len = 0;
    char c = peek_char(vm);
    while (!isspace(c) && c != EOF && c != '(') {
        if (len < 1023) {
            vm->curword[len] = c;
            len++;
        }
        vm_getc(vm);
        c = peek_char(vm);
    }
    vm->curword[len] = '\0';
}

data parse_number(struct forthvm *vm)
{
    parse_word(vm);
    char *endp;
    long num = strtol(vm->curword, &endp, 10);
    return num;
}

static void escape(struct forthvm *vm, StrBuilder *sb)
{
    char c = vm_getc(vm);
    switch (c) {
    case 't':
        sb_appendc(sb, '\t');
        break;
    case 'n':
        sb_appendc(sb, '\n');
        break;
    case '\\':
        sb_appendc(sb, '\\');
        break;
    case '"':
        sb_appendc(sb, '"');
        break;
    default:
        sb_appendc(sb, '\\');
        sb_appendc(sb, c);
    }
}

char *parse_string(struct forthvm *vm)
{
    vm_getc(vm);
    char c = vm_getc(vm);
    StrBuilder sb;
    sb_init(&sb);
    while (1) {
        switch (c) {
        case EOF:
            return NULL;
        case '\\':
            escape(vm, &sb);
            break;
        case '"':
            sb_appendc(&sb, '\0');
            return sb.buf;
        default:
            sb_appendc(&sb, c);
        }
        c = vm_getc(vm);
    }
}

struct token get_token(struct forthvm *vm)
{
    struct token tok = {TOK_INVALID, 0};
    while (1) {
        skipspace(vm);
        char c = vm_getc(vm);
        char c1 = vm_getc(vm);
        vm_ungetc(vm, c1);
        vm_ungetc(vm, c);
        if (c >= '0' && c <= '9' || c == '-' && !isspace(c1)) {
            tok.type = TOK_NUM;
            tok.dat = parse_number(vm);
            return tok;
        } else if (c == EOF) {
            tok.type = TOK_EOF;
            return tok;
        } else if (c == '(') {
            skipcomment(vm);
        } else if (c == '"') {
            char *s = parse_string(vm);
            if (s == NULL)
                continue;
            tok.type = TOK_NUM;
            tok.dat = (data)s;
            return tok;
        } else {
            parse_word(vm);
            int syn_num = get_syntax(vm->curword);
            if (syn_num >= 0) {
                tok.type = TOK_SYNTAX;
                tok.dat = syn_num;
            } else {
                tok.type = TOK_WORD;
            }
            return tok;
        }
    }
    return tok;
}
