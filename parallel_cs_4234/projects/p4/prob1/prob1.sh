SIZE=$1
NODES=$2

mpirun -np $NODES -map-by ppr:1:node -hostfile ../rlogin_nodes ./prob1 $SIZE data/matrix_$SIZE_a.txt data/matrix_$SIZE_b.txt

