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

#include "vm.h"

int main(int argc, char **argv)
{
    struct forthvm vm;
    int ret;
    char *filename = "stdin";
    FILE *fin = stdin;
    if (argc > 1) {
        fin = fopen(argv[1], "r");
        if (fin == NULL) {
            fprintf(stderr, "Failed to open file: %s\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        filename = argv[1];
    }
    vm_init(&vm, fin, stdout);
    vm_run(&vm);
    if (vm.ret == -2) {
        fprintf(stderr, "Assertion failed: %s:%d\n", filename, vm.linenum);
    } else if (vm.ret < 0){
        fprintf(stderr, "VM error: %s\n", vm.errmsg);
    }
    return vm.ret;
}
