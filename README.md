# reinforth

**!!! WORK IN PROGRESS !!!**

A small implementation of Forth supporting recursion for fun.

## Example

A recursive fibonacci function:

```
: fibo (recursive fibonacci)
    dup 2 < if
        1 swap drop
    else
        dup 1 - fibo over 2 - fibo + swap drop
    then
;

32 fibo .
```

