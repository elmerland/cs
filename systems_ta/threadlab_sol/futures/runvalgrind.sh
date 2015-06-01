#!/bin/bash -x

CMD="/home/courses/cs3214/valgrind-3.10.0-install/bin/valgrind --tool=helgrind --suppressions=suppress-dubious-signal"

${CMD} ./threadpool_test
${CMD} ./threadpool_test -n 1
${CMD} ./threadpool_test -n 2
${CMD} ./threadpool_test2
${CMD} ./threadpool_test2 -n 1
${CMD} ./threadpool_test2 -n 2
${CMD} ./threadpool_test3
${CMD} ./threadpool_test3 -n 1
${CMD} ./threadpool_test3 -n 2
${CMD} ./quicksort -n 4 10000
${CMD} ./mergesort -n 4 10000
${CMD} ./fib_test -n 4 10
