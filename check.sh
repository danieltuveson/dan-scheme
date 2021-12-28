# Script repeatedly checks syntax of file and looks for errors
filename=main.c
while true; do
    m1=$(md5 "$filename")

    if [ "$m1" != "$m2" ] ; then
        clear
        cc -Wextra -Wall -fsyntax-only main.c && echo "All good"
    fi
    m2=$(md5 "$filename")
    sleep 1
done
