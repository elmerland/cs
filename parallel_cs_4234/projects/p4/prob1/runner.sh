SIZE=$1
NODES=$2
a='_a'
b='_b'
prefix='data/matrix_'
postfix='.txt'
file_a=$prefix$SIZE$a$postfix
file_b=$prefix$SIZE$b$postfix

mpirun -np $NODES -map-by ppr:1:node -hostfile ../rlogin_nodes ./prob1 $SIZE $file_a $file_b

