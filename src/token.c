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

#include "syntax.h"

char peek_char(FILE *fp)
{
    int c = fgetc(fp);
    if (c == EOF)
        return c;
    ungetc(c, fp);
    return c;
}

void skipspace(FILE *fp)
{
    char c = peek_char(fp);
    while (c == ' ' || c == '\t' || c == '\n' || c== '\r') {
        fgetc(fp);
        c = peek_char(fp);
    }
}

void skipcomment(FILE *fp)
{
    char c = peek_char(fp);
    while (c != ')') {
        fgetc(fp);
        c = peek_char(fp);
    }
    fgetc(fp);
}

void parse_word(FILE *fp, char *buf)
{
    int len = 0;
    char c = peek_char(fp);
    while (!isspace(c) && c != EOF && c != '(') {
        if (len < 1023) {
            buf[len] = c;
            len++;
        }
        fgetc(fp);
        c = peek_char(fp);
    }
    buf[len] = '\0';
}

data parse_number(FILE *fp, char *buf)
{
    parse_word(fp, buf);
    char *endp;
    long num = strtol(buf, &endp, 10);
    return num;
}

struct token get_token(FILE *fp, char *buf)
{
    struct token tok = {TOK_INVALID, 0}; 
    while (1) {
        skipspace(fp);
        char c = peek_char(fp);
        if (c >= '0' && c <= '9') {
            tok.type = TOK_NUM;
            tok.dat = parse_number(fp, buf);
            return tok;
        } else if (c == EOF) {
            tok.type = TOK_EOF;
            return tok;
        } else if (c == '(') {
            skipcomment(fp);
        } else {
            parse_word(fp, buf);
            int syn_num = get_syntax(buf);
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
