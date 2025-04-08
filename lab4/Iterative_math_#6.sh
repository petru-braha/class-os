#!/bin/bash
fibonacci() 
{
    if [ $1 -le 1 ]; then
        echo $1
    else
        a=0
        b=1
        for ((i=2; i <= $1; i++)) do
            fn=$((a + b))
            a=$b
            b=$fn
        done
        echo $fn
    fi
}

if [$# -eq 0 ]; then
    read -p "index==" n
elif [ $# -gt 1 ]; then
    echo "insert only one parameter"
else
    n=$1
fi

fibonacci $n
# works for [1, 103]
# echo if wrong number of parameters
# needs revision