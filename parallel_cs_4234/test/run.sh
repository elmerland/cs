prob=$1
np=$2

mpirun -np $np -hostfile ./rlogin_nodes ./prob$prob
