#!/bin/sh

size=$1
result='data/matrix_'$1'_c.txt'

diff output.txt $result
