P=$1

N1=1056
N2=2112
N3=4224

if [ "$P" = '1' ]; then
  ./runner.sh -e -p 1 -n $N1
  ./runner.sh -e -p 1 -n $N2
  ./runner.sh -e -p 1 -n $N3

  ./runner.sh -e -p 1 -n $N1 -a
  ./runner.sh -e -p 1 -n $N2 -a
  ./runner.sh -e -p 1 -n $N3 -a

  
fi