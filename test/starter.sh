#!/bin/bash

# input validation
if [ $# -ne 1 ]; then
  echo "error: wrong number of parameters"
  exit 1
fi

# compilation
rm -f fifo
gcc ./coordonator/supervisor.c -o ./coordonator/supervisor.out
if [ $? -ne 0 ]; then
  echo "error: compilation failed"
  exit 3
fi

gcc ./subordinates/worker1.c -o ./subordinates/worker1.out -lm # math lib
if [ $? -ne 0 ]; then
  echo "error: compilation failed"
  exit 3
fi

gcc ./subordinates/worker2.c -o ./subordinates/worker2.out
if [ $? -ne 0 ]; then
  echo "error: compilation failed"
  exit 3
fi

# program launch
p=$1
file="./input_data.txt"
if [ $p -eq 1 ]; then
  ./subordinates/worker1.out $file &
  sleep 2
  ./subordinates/worker2.out

elif [ $p -eq 2 ]; then
  ./subordinates/worker2.out 1 &
  sleep 3
  ./subordinates/worker1.out $file

else
  echo "error: wrong paramter"
  exit 2
fi

# todo clean the workspace
echo "all three processes ended"
# echo conținutul mapării nepersistente cu nume
# delete maparea nepersistentă cu nume
rm -f fifo
