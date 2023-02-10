#!/usr/bin/env bash

for var in "$@"; do
    ./reinforth $var
    if [ $? -ne 0 ]; then
        exit 255
    fi
done

