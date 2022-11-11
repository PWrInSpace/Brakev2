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
-build/header_guard,\
-readability/casting
"

cpplint ${OPTIONS} --filter=${FILTERS} ${FILES}