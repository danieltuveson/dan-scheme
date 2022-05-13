#!/bin/zsh
clear
exe=danlisp
current_dir=$(pwd)
directories=('lexer') # 'parser')

# Generate function headers for all *.c files
for directory in $directories; do
    for file in $directory/*.c; do
        echo "... generating function headers for $file ..."
        file_name=${file:0:-2}
        dir=${0:a:h}
        zsh ~$dir/generate_function_declaration.sh $file_name 
        echo $file_name
    done
    # zsh ~$dir/generate_function_declaration.sh $file_name 
    # if [[ $file_name != "main" ]]; then
    #     zsh ~$dir/generate_function_declaration.sh $file_name 
    # fi
done

echo "... compiling ..."
cc -I$current_dir/lexer lexer/*.c *.c -Wall -Wextra -o $exe
if [[ $? -eq 0 ]]; then
    echo "... beginning execution of $exe ..."
    ./$exe
fi
