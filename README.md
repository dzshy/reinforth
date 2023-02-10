# reinforth

**!!! WORK IN PROGRESS !!!**

A small implementation of Forth for fun.

## Build

Debug version:

```
make
```

Release version:

```
RELEASE=1 make
```

## Examples

### Hello, World

```forth
"hello, world" print cr
```

### Recursion

```forth
: fibo ( recursive fibonacci )
    dup 2 < if
        1 swap drop
    else
        dup 1 - fibo over 2 - fibo + swap drop
    then
;

32 fibo .
```

```forth
( mutual recurtion )
: is_even
    dup 0 = if
        drop 1
    else
        1 - is_odd
    then
;

: is_odd
    dup 0 = if
        drop 0
    else
        1 - is_even
    then
;

15 is_odd  .
15 is_even .
20 is_odd  .
20 is_even .
```

```forth
( stackoverflow )
: count 1 + dup . count ;

0 count
```

### Variable

```forth
: variable create 1 allot ;
variable var
42 var !
var @ .
```

