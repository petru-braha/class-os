#!/bin/bash
aranjaments()   
{
    numerator=1
    numitor=1
    for ((i=1; i <= $1; i++)) do
        numerator=$numerator * $i 
        if [i -eq $2]; then
            numitor=$numerator
        fi
    done
    
    echo $numerator $numitor
}

if [$# -gt 2]; then
    echo "too many"
elif [$# -lt 2]; then
    echo "no paramenters"
else
    n=$1
    k=$2
    echo "A($n,$k) = $(aranjaments $n $k)"
fi