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

#ifndef NEBUTIL_STR_H_
#define NEBUTIL_STR_H_

#include <stdio.h>

char *str_strip(char *str);
char **str_split(char *str, char delim);
void str_list_free(char **list);

typedef struct {
    char *buf;
    int size;
    int cap;
} StrBuilder;

void sb_init(StrBuilder *sb);
void sb_append(StrBuilder *sb, char *format, ...);
void sb_appendc(StrBuilder *sb, char c);

char *fgetline(FILE *fp);
int fpeek(FILE *fp);

#endif
