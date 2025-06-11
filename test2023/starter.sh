#!/bin/bash

# parameters
if [ $# -ne 1 ]; then
  echo "error: wrong number of arguments"
  exit 1
fi

p=$1
if [ $p -ne 1 ] && [ $p -ne 2 ]; then
  echo "error: wrong argument"
  exit 2
fi

file="input_data.txt"

# compilation
rm -f fifo_test2023
gcc ./manager/supervisor.c -o ./manager/supervisor.out
if [ $? -ne 0 ]; then
  echo "error: compilation failed"
  exit 3
fi

gcc ./workers/worker1.c -o ./workers/worker1.out
if [ $? -ne 0 ]; then
  echo "error: compilation failed"
  exit 3
fi

gcc ./workers/worker2.c -o ./workers/worker2.out
if [ $? -ne 0 ]; then
  echo "error: compilation failed"
  exit 3
fi

# actual run
if [ $p -eq 1 ]; then
  ./workers/worker2.out &
  sleep 2
  ./manager/supervisor.out $file
elif [ $p -eq 2 ]; then
  ./manager/supervisor.out $file &
  sleep 3
  ./workers/worker2.out
fi

# delete remainders
rm -f /dev/shm/shared_memory
rm -f fifo_test2023

