#!/bin/sh

N='' # Size of array
P='' # Number of processes
S='' # Asynchronous flag
B='' # Buffered flag
M='' # Multiplier
T='' # Thread count
O='' # Pthread flag
G='-g' # Generate flag
E='-e' # Timer flag
H='' # Help flag (Prints usage)
F=''
V='' # Verbose flag

while getopts 'n:p:abm:t:ohvf:' flag; do
  case "${flag}" in
    n) N="-n ${OPTARG}" ;;
    p) P="-np ${OPTARG}" ;;
    a) S='-a' ;;
    b) B='-b' ;;
    m) M="-m ${OPTARG}" ;;
    t) T="-t ${OPTARG}" ;;
    o) O="-o" ;;
    h) H='-h' ;;
    f) F="${OPTARG}" ;;
    v) V='true' ;;
    *) error "Unexpected option ${flag}" ;;
  esac
done

F='strong'$F'.txt'

run="mpirun $P -hostfile ../rlogin_nodes ./prob1 $N $S $B $M $T $O $G $E $H > $F"

if [ "$V" = 'true' ]; then
  echo $run
fi
echo $run

eval $run
