for FILE in bugs*.c
do
    echo gcc -O2 -S -g -o `basename $FILE .c`_.O2 -Wall $FILE
    gcc -O2 -S -g -o `basename $FILE .c`_.O2 -Wall $FILE
    echo gcc -S -g -o `basename $FILE .c`_.O0 -Wall $FILE
    gcc -S -g -o `basename $FILE .c`_.O0 -Wall $FILE
done
