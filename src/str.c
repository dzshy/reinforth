/* Copyright (c) 2022-2023, ~dzshy <dzshy@outlook.com>
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

#include "str.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **str_split(char *str, char delim)
{
    char **ret;

    if (str == NULL)
        return NULL;
    if (*str == '\n') {
        ret = malloc(sizeof(char *));
        *ret = NULL;
        return ret;
    }
    int count = 0;
    char *begin = str;
    for (char *p = str; *p != '\0'; p++) {
        if (*p != delim && !(delim == '\0' && isspace(*p))) {
            continue;
        }
        int size = p - begin;
        if (size > 0)
            count++;
    }
    count++;
    ret = malloc((count + 1) * sizeof(char *));
    memset(ret, 0, (count + 1) * sizeof(char *));

    begin = str;
    int i = 0;
    bool finished = false;
    for (char *p = str; !finished; p++) {
        if (*p == '\0')
            finished = true;
        if (*p != delim && *p != '\0' && !(delim == '\0' && isspace(*p))) {
            continue;
        }
        int size = p - begin;
        if (size == 0) {
            begin = p + 1;
            continue;
        }
        char *buf = malloc(sizeof(char) * (size + 1));
        buf[size] = '\0';
        memcpy(buf, begin, size * sizeof(char));
        begin = p + 1;
        ret[i] = buf;
        i++;
    }
    return ret;
}

void str_list_free(char **list)
{
    char **p = list;
    while (*p != NULL) {
        free(*p);
        p++;
    }
    free(list);
}

char *str_strip(char *str)
{
    int len = strlen(str);
    char *begin = str;
    char *end = str + len - 1;
    while (isspace(*begin) && begin < end) {
        begin++;
    }
    while (isspace(*end) && end >= begin) {
        end--;
    }
    len = end - begin + 1;
    char *buf = malloc(sizeof(char) * (len) + 1);
    buf[len] = '\0';
    memcpy(buf, begin, len);
    return buf;
}

void sb_init(StrBuilder *sb)
{
    *sb = (StrBuilder){.size = 0, .cap = 16};
    sb->buf = malloc(sizeof(char) * 17);
}

static void sb_reserve(StrBuilder *sb, int extra)
{
    if (sb->size + extra <= sb->cap) {
        return;
    }
    int new_cap = (sb->size + extra) * 2;
    sb->buf = realloc(sb->buf, new_cap + 1);
    memset(sb->buf + sb->cap, 0, new_cap - sb->cap + 1);
    sb->cap = new_cap;
}

void sb_append(StrBuilder *sb, char *format, ...)
{
    va_list va1;
    va_list va2;
    va_start(va1, format);
    va_copy(va2, va1);
    int size = vsnprintf(NULL, 0, format, va1);
    sb_reserve(sb, size);
    vsnprintf(sb->buf + sb->size, sb->cap - sb->size + 1, format, va2);
}

void sb_appendc(StrBuilder *sb, char c)
{
    sb_reserve(sb, 1);
    sb->buf[sb->size] = c;
    sb->size++;
}

char *fgetline(FILE *fp)
{
    StrBuilder sb;
    sb_init(&sb);
    while (true) {
        int c = fgetc(fp);
        if (c == EOF && sb.size == 0)
            return NULL;
        if (c != EOF)
            sb_appendc(&sb, c);
        if (c == EOF || c == '\n')
            return sb.buf;
    }
    return NULL;
}

int fpeek(FILE *fp)
{
    int c = fgetc(fp);
    if (c == EOF)
        return c;
    ungetc(c, fp);
    return c;
}
