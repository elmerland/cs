#!/bin/sh

for N in 1056 2112 4224
do
  cmd="./time.sh -p 2 -n $N > strong1a_2.txt"
  echo cmd
done
