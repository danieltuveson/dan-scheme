#!/bin/zsh
clear

exe=danlisp

# Generate function headers for all *.c files
for file in *.c; do
    echo "... generating function headers for $file ..."
    file_name=${file:0:-2}
    dir=${0:a:h}
    if [[ $file_name != "main" ]]; then
        #echo $dir
        zsh ~$dir/generate_function_declaration.sh $file_name 
        #zsh ~/Users/dtuveson/Programming-Projects/my-lisp/rewrite/generate_function_declaration.sh $file_name 
    fi
done

echo "... compiling ..."
cc *.c -Wall -Wextra -o $exe
if [[ $? -eq 0 ]]; then
    echo "... beginning execution of $exe ..."
    ./$exe
fi
