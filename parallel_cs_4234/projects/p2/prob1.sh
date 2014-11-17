#!/bin/bash
THREADS=$1
NUM=$2
while [ $NUM -le $3 ]
do
	./prob1 $THREADS $NUM
	let "NUM *= 2"
done
