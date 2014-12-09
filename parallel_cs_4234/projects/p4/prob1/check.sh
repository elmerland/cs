#!/bin/sh

N='' # Size of array
P='' # Number of processes
S='' # Asynchronous flag
B='' # Buffered flag
M='' # Multiplier
T='' # Thread count
O='' # Pthread flag
G='' # Generate flag
E='' # Timer flag
H='' # Help flag (Prints usage)
L='10'
V='' # Verbose flag

while getopts 'n:p:abm:t:ogehvl:' flag; do
  case "${flag}" in
    n) N="${OPTARG}" ;;
    p) P="-p ${OPTARG}" ;;
    a) S='-a' ;;
    b) B='-b' ;;
    m) M="-m ${OPTARG}" ;;
    t) T="-t ${OPTARG}" ;;
    o) O="-o" ;;
    g) G="-g" ;;
    e) E='-e' ;;
    h) H='-h' ;;
  	l) L="${OPTARG}" ;;
  	v) V='-v' ;;
    *) error "Unexpected option ${flag}" ;;
  esac
done

diff="./mdiff.sh $N > diff.txt"
N="-n $N"
runner="./runner.sh $N $P $S $B $M $T $O $G $E $H > output.txt 2>&1"

if [ "$V" = '-v' ]; then
	echo $runner
fi

eval $runner

if [ "$V" = '-v' ]; then
	echo $diff
fi

eval $diff
printf "Diff lines\t"
cat diff.txt | wc -l
echo "First $L lines"
head -n $L diff.txt
