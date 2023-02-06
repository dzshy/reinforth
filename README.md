# reinforth

**!!! WORK IN PROGRESS !!!**

A small implementation of Forth for fun.

## Examples

### Recursion

Unlike other Forth, you can do recursion.

```
: fibo ( recursive fibonacci )
    dup 2 < if
        1 swap drop
    else
        dup 1 - fibo over 2 - fibo + swap drop
    then
;

32 fibo .
```

And it's unlimited. See how deep you can get before OOM:

```
: count 1 + dup . count ;

0 count
```

## Use before declaration

You can use bar before it's declaration.

```
: foo bar ;
: bar 42 . ;

foo
```

But be careful not to get a undefined word:

```
: foo a_not_defined_word ;

foo ( error! )
```

