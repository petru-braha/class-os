#!/bin/bash

if [ !# -ne 2 ]; then exit 1 fi

p = $1

if [ p -eq 1 ]; then 
{
    ./subordinates/worker1 input_data.txt &
    sleep 2
    ./subordinates/worker2
} fi

if [ p -eq 2 ]; then
{
    ./subordinates/worker1 input_data.txt &
    sleep 3
    ./subordinates/worker1 
} fi

# wait
# echo //
