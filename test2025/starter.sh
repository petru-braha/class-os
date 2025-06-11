#!/bin/bash

# parameters
if [ $# -ne 1 ]; then
  echo "error: wrong number of arguments"
  exit 1
fi

p=$1
if [ $p -ne 0 ] && [ $p -ne 1 ]; then
  echo "error: wrong argument"
  exit 2
fi

file="data/input.txt"

# compilation
rm -f fifo
gcc app/analyzers/analyzer.c -o app/analyzers/analyzer.out
if [ $? -ne 0 ]; then
  echo "error: compilation failed"
  exit 3
fi

gcc app/services/service.c -o app/services/service.out
if [ $? -ne 0 ]; then
  echo "error: compilation failed"
  exit 3
fi

gcc app/services/worker.c -o app/services/worker.out
if [ $? -ne 0 ]; then
  echo "error: compilation failed"
  exit 3
fi

# actual run
if [ $p -eq 0 ]; then
  ./app/analyzers/analyzer.out $file &
  sleep 1
  ./app/services/service.out
elif [ $p -eq 1 ]; then
  ./app/services/service.out &
  sleep 2
  ./app/analyzers/analyzer.out $file
fi

# delete remainders
rm -f /dev/shm/mem_shrd
rm -f fifo
cat /dev/shm/mem_shrd
