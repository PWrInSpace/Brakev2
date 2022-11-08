#!/bin/bash

FILES="\
main/main.c
components
"

OPTIONS="\
--recursive
"

FILTERS="\
-build/include_subdir,\
-build/header_guard
"

cpplint ${OPTIONS} --filter=${FILTERS} ${FILES}