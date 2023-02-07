#!/bin/bash

if [[ $RELEASE -eq 1 ]] ; then
    echo -O2 -flto
    exit
fi

echo -g

