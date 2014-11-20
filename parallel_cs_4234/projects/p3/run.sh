OUT=$1
MEM=$2

echo "1 kb & " >> $OUT
./prob3 $MEM 16 data_files/01* >> $OUT
./prob3_linear data_files/01* >> $OUT

echo "10 kb & " >> $OUT
./prob3 $MEM 16 data_files/02* >> $OUT
./prob3_linear data_files/02* >> $OUT

echo "100 kb & " >> $OUT
./prob3 $MEM 16 data_files/03* >> $OUT
./prob3_linear data_files/03* >> $OUT

echo "1 mb & " >> $OUT
./prob3 $MEM 16 data_files/04* >> $OUT
./prob3_linear data_files/04* >> $OUT

echo "10 mb & " >> $OUT
./prob3 $MEM 16 data_files/05* >> $OUT
./prob3_linear data_files/05* >> $OUT

echo "100 mb & " >> $OUT
./prob3 $MEM 16 data_files/06* >> $OUT
./prob3_linear data_files/06* >> $OUT

echo "1 gb & " >> $OUT
./prob3 $MEM 16 data_files/07* >> $OUT
./prob3_linear data_files/07* >> $OUT

echo "5 gb & " >> $OUT
./prob3 $MEM 16 data_files/08* >> $OUT
./prob3_linear data_files/08* >> $OUT