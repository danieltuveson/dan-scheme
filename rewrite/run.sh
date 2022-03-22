#!/bin/zsh
clear

for file in *.c; do
    file_name=${file:0:-2}
    if [[ $file_name != "main" ]]; then
        zsh generate_function_declarations.sh $file_name
    fi
done
# cc *.c -o danlisp
# ./danlisp
