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

15 is_odd 
assert

15 is_even
not assert

20 is_odd
not assert

20 is_even
assert

depth 0 = assert

