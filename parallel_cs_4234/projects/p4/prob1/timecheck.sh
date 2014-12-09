#!/bin/sh
i=2
j=1

T='false'
V='false'

while getopts 'tv' flag; do
  case "${flag}" in
    t) T='true' ;;
	v) V='true' ;;
    *) error "Unexpected option ${flag}" ;;
  esac
done

if [ T = 'true' ]; then
	make compilet
else
	make compile
fi


while [ $i -le 4096 ]
do
	let "j=1"
	while [ $j -le 32 -a $j -le $i ]
	do
		echo 'N:' $i 'P:' $j
		runner -n $i -p $j
		let "j=j*2"
	done
	let "i=i*2"
done
