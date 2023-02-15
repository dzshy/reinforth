: vec3-add
    0 pick >r 3 pick r> + >r
    1 pick >r 4 pick r> + >r
    2 pick >r 5 pick r> + >r
    6 0 do drop loop
    r> r> r>
;

1 2 3
4 5 6
vec3-add

9 = assert
7 = assert
5 = assert

