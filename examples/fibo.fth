: fibo ( recursive fibonacci )
    dup 2 < if
        1 swap drop
    else
        dup 1 - fibo over 2 - fibo + swap drop
    then
;

32 fibo .

